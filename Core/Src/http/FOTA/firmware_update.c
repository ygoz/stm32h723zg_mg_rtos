/*
 * firmware_update.c
 *
 *  Created on: Mar 18, 2025
 *      Author: yam
 */

#include "mongoose.h"
#include "http/FOTA/firmware_update.h"
#include "http/auth/users.h"


#define MAX_FILE_NAME 32



struct upload_state {
  char marker;               // Tells that we're a file upload connection
  size_t expected;           // POST data length, bytes
  size_t received;           // Already received bytes
  void *fp;                  // Opened file
  bool (*fn_close)(void *);  // Close function
  bool (*fn_write)(void *, void *, size_t);  // Write function
};


struct apihandler {
  const char *name;
  const char *type;
  bool readonly;
  int read_level;
  int write_level;
  unsigned long version;
};


typedef void *(*mongoose_ota_opener_t)(char *, size_t);
typedef bool (*mongoose_ota_closer_t)(void *);
typedef bool (*mongoose_ota_writer_t)(void *, void *, size_t);



struct apihandler_upload {
  struct apihandler common;
  void *(*opener)(char *, size_t);         // Open function (OTA and upload)
  bool (*closer)(void *);                  // Closer function (OTA and upload)
  bool (*writer)(void *, void *, size_t);  // Writer function (OTA and upload)
};




struct apihandler_ota {
  struct apihandler common;
  void *(*opener)(char *, size_t);         // Open function (OTA and upload)
  bool (*closer)(void *);                  // Closer function (OTA and upload)
  bool (*writer)(void *, void *, size_t);  // Writer function (OTA and upload)
};


void *glue_ota_begin_firmware_update(char *file_name, size_t total_size) {
  bool ok = mg_ota_begin(total_size);
  MG_DEBUG(("%s size %lu, ok: %d", file_name, total_size, ok));
  return ok ? (void *) 1 : NULL;
}
bool glue_ota_end_firmware_update(void *context) {
  return mg_ota_end();;
}
bool glue_ota_write_firmware_update(void *context, void *buf, size_t len) {
  MG_DEBUG(("ctx: %p %p/%lu", context, buf, len));
  return mg_ota_write(buf, len);
}

struct apihandler_ota s_apihandler_firmware_update = {{"firmware_update", "ota", false, 3, 7, 0UL}, glue_ota_begin_firmware_update, glue_ota_end_firmware_update, glue_ota_write_firmware_update};




static void close_uploaded_file(struct upload_state *us) {
  us->marker = 0;
  if (us->fn_close != NULL && us->fp != NULL) {
    us->fn_close(us->fp);
    us->fp = NULL;
  }
  memset(us, 0, sizeof(*us));
}



static void upload_handler(struct mg_connection *c, int ev, void *ev_data) {
  struct upload_state *us = (struct upload_state *) c->data;
  if (sizeof(*us) > sizeof(c->data)) {
    mg_error(
        c, "FAILURE: sizeof(c->data) == %lu, need %lu. Set -DMG_DATA_SIZE=XXX",
        sizeof(c->data), sizeof(*us));
    return;
  }
  // Catch uploaded file data for both MG_EV_READ and MG_EV_HTTP_HDRS
  if (us->marker == 'U' && ev == MG_EV_READ && us->expected > 0 &&
      c->recv.len > 0) {
    size_t alignment = 512;  // Maximum flash write granularity (iMXRT, Pico)
    size_t aligned = (us->received + c->recv.len < us->expected)
                         ? aligned = MG_ROUND_DOWN(c->recv.len, alignment)
                         : c->recv.len;  // Last write can be unaligned
    bool ok = aligned > 0 ? us->fn_write(us->fp, c->recv.buf, aligned) : true;
    us->received += aligned;
    MG_DEBUG(("%lu chunk: %lu/%lu, %lu/%lu, ok: %d", c->id, aligned,
              c->recv.len, us->received, us->expected, ok));
    mg_iobuf_del(&c->recv, 0, aligned);  // Delete received data
    if (ok == false) {
      mg_http_reply(c, 400, "", "Upload error\n");
      close_uploaded_file(us);
      c->is_draining = 1;  // Close connection when response it sent
    } else if (us->received >= us->expected) {
      // Uploaded everything. Send response back
      MG_INFO(("%lu done, %lu bytes", c->id, us->received));
      mg_http_reply(c, 200, NULL, "%lu ok\n", us->received);
      close_uploaded_file(us);
      c->is_draining = 1;  // Close connection when response it sent
    }
  }

  // Close uploading file descriptor
  if (us->marker == 'U' && ev == MG_EV_CLOSE) close_uploaded_file(us);
  (void) ev_data;
}



static void get_file_name_from_uri(struct mg_str uri, char *buf, size_t len) {
  struct mg_str parts[3];
  memset(parts, 0, sizeof(parts));           // Init match parts
  mg_match(uri, mg_str("/api/*/#"), parts);  // Fetch file name
  mg_url_decode(parts[1].buf, parts[1].len, buf, len, 0);
}






static void prep_upload(struct mg_connection *c, struct mg_http_message *hm,
                        void *(*fn_open)(char *, size_t),
                        bool (*fn_close)(void *),
                        bool (*fn_write)(void *, void *, size_t)) {
  struct upload_state *us = (struct upload_state *) c->data;


  char path[MAX_FILE_NAME];
  get_file_name_from_uri(hm->uri, path, sizeof(path));



  // const char* path = "mg_rtos.bin";
  memset(us, 0, sizeof(*us));  // Cleanup upload state
  us->fp = fn_open(path, hm->body.len);
  MG_DEBUG(("file: [%s] size: %lu fp: %p", path, hm->body.len, us->fp));
  us->marker = 'U';  // Mark us as an upload connection
  if (us->fp == NULL) {
    mg_http_reply(c, 400, JSON_HEADERS, "File open error\n");
    c->is_draining = 1;
  } else {
    us->expected = hm->body.len;              // Store number of bytes we expect
    us->fn_close = fn_close;                  // Store closing function
    us->fn_write = fn_write;                  // Store writing function
    mg_iobuf_del(&c->recv, 0, hm->head.len);  // Delete HTTP headers
    c->fn = upload_handler;                   // Change event handler function
    c->pfn = NULL;                            // Detach HTTP handler
    mg_call(c, MG_EV_READ, &c->recv.len);     // Write initial data
  }
}





void handle_uploads(struct mg_connection *c, int ev, void *ev_data) {
  struct upload_state *us = (struct upload_state *) c->data;
  struct mg_http_message *hm = (struct mg_http_message *) ev_data;

  if (ev == MG_EV_HTTP_HDRS && us->marker == 0 &&
      mg_strcmp(hm->method, mg_str("POST")) == 0) {
    // Match against your OTA upload endpoint
    if (mg_match(hm->uri, mg_str("/api/firmware_update/*"), NULL)) {

		struct user *current_user = authenticate(hm);
		if (current_user == NULL){
			mg_http_reply(c, 404, "", "user not found\n");
		}
		else{
        prep_upload(c, hm,
              s_apihandler_firmware_update.opener,
              s_apihandler_firmware_update.closer,
              s_apihandler_firmware_update.writer);
      }
      // Directly prep the upload with your static handler

    }
  }
}
