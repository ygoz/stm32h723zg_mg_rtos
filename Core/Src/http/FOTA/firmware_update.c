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




void *begin_firmware_update(char *file_name, size_t total_size) {
  bool ok = mg_ota_begin(total_size);
  MG_DEBUG(("%s size %lu, ok: %d", file_name, total_size, ok));
  return ok ? (void *) 1 : NULL;
}
bool end_firmware_update(void *context) {
  return mg_ota_end();;
}
bool write_firmware_update(void *context, void *buf, size_t len) {
  MG_DEBUG(("ctx: %p %p/%lu", context, buf, len));
  return mg_ota_write(buf, len);
}



/**
 * @brief Finalizes and cleans up the current upload state.
 *
 * This function is responsible for closing an ongoing file upload:
 * - It clears the upload marker from connection data.
 * - If a close function is provided and a valid file pointer exists,
 *   it calls the close function to finalize the file.
 * - It then resets the entire upload_state struct to zero.
 *
 * @param us Pointer to the upload_state struct managing the current upload.
 */
static void close_uploaded_file(struct upload_state *us) {
  us->marker = 0;  // Clear marker to indicate upload is no longer active

  // If a valid close function and file pointer are set, close the file
  if (us->fn_close != NULL && us->fp != NULL) {
    us->fn_close(us->fp);  // Finalize file
    us->fp = NULL;         // Clear file pointer
  }

  // Reset the entire upload_state structure
  memset(us, 0, sizeof(*us));
}



/**
 * @brief Upload handler for processing HTTP file uploads.
 *
 * This handler is assigned during file upload initialization. It processes
 * incoming data chunks (MG_EV_READ), writes aligned data using the user-defined
 * write function, and replies with status once the full upload is complete.
 * It also handles upload cleanup on connection close (MG_EV_CLOSE).
 *
 * @param c        The Mongoose connection.
 * @param ev       The event type (MG_EV_READ, MG_EV_CLOSE, etc.).
 * @param ev_data  Pointer to event-specific data (unused here).
 */
static void upload_handler(struct mg_connection *c, int ev, void *ev_data) {
  struct upload_state *us = (struct upload_state *) c->data;

  // Sanity check: Ensure upload_state fits inside the connection data buffer
  if (sizeof(*us) > sizeof(c->data)) {
    mg_error(
        c, "FAILURE: sizeof(c->data) == %lu, need %lu. Set -DMG_DATA_SIZE=XXX",
        sizeof(c->data), sizeof(*us));
    return;
  }

  // Handle incoming upload data during MG_EV_READ (upload in progress)
  if (us->marker == 'U' && ev == MG_EV_READ && us->expected > 0 &&
      c->recv.len > 0) {
    // Flash write alignment requirement, for this device any num that doesn't cause oom error works.
    size_t alignment = 512;
    size_t aligned = (us->received + c->recv.len < us->expected)
                         ? MG_ROUND_DOWN(c->recv.len, alignment)
                         : c->recv.len;  // Last chunk may be unaligned

    // Write aligned chunk using user-supplied function
    bool ok = aligned > 0 ? us->fn_write(us->fp, c->recv.buf, aligned) : true;
    us->received += aligned;

    MG_DEBUG(("%lu chunk: %lu/%lu, %lu/%lu, ok: %d", c->id, aligned,
              c->recv.len, us->received, us->expected, ok));

    // Remove consumed bytes from the receive buffer
    mg_iobuf_del(&c->recv, 0, aligned);

    if (ok == false) {
      // Error occurred during write
      mg_http_reply(c, 400, "", "Upload error\n");
      close_uploaded_file(us);
      c->is_draining = 1;  // Close connection after response
    } else if (us->received >= us->expected) {
      // Upload complete
      MG_INFO(("%lu done, %lu bytes", c->id, us->received));
      mg_http_reply(c, 200, NULL, "%lu ok\n", us->received);
      close_uploaded_file(us);
      c->is_draining = 1;  // Close connection after response
    }
  }

  // Handle cleanup when connection closes
  if (us->marker == 'U' && ev == MG_EV_CLOSE) close_uploaded_file(us);
  (void) ev_data;  // Unused
}



/**
 * @brief Extracts a file name from an HTTP URI and decodes it.
 *
 * This function parses a URI like `/api/firmware_update/filename.bin` and
 * extracts the `filename.bin` part into a null-terminated buffer.
 *
 * Example:
 *   Input:  "/api/upload/my_file.hex"
 *   Output: buf = "my_file.hex"
 *
 * @param uri The input URI (as `mg_str`) from the HTTP request.
 * @param buf Destination buffer to store the decoded file name.
 * @param len Size of the destination buffer.
 */
static void get_file_name_from_uri(struct mg_str uri, char *buf, size_t len) {
  struct mg_str parts[3];

  // Clear the array of parts to make sure it’s fully initialized
  memset(parts, 0, sizeof(parts));

  // Use Mongoose's simple glob-style matcher to extract parts from the URI
  // Pattern: "/api/*/#" matches URIs like "/api/section/filename"
  // Result: parts[1] = "section", parts[2] = "filename"
  mg_match(uri, mg_str("/api/*/#"), parts);

  // URL-decode the extracted file name into `buf` (null-terminated)
  // parts[2] contains the actual filename (e.g. "my_file.hex")
  mg_url_decode(parts[2].buf, parts[2].len, buf, len, 0);
}





/**
 * @brief Prepares an HTTP connection for streaming file upload.
 *
 * This function sets up the connection `c` to handle an HTTP POST upload request.
 * It initializes the upload state, extracts the file name from the URI, and assigns 
 * custom file handling functions. If initialization succeeds, it replaces the HTTP 
 * handler with a custom upload handler to process the incoming file body incrementally.
 *
 * @param c         Pointer to the active Mongoose connection.
 * @param hm        Parsed HTTP request containing headers and body.
 * @param fn_open   Function to open a file or buffer for writing.
 * @param fn_close  Function to close the file/buffer after writing is complete.
 * @param fn_write  Function to write a chunk of data to the file/buffer.
 */
static void prep_upload(struct mg_connection *c, struct mg_http_message *hm,
                        void *(*fn_open)(char *, size_t),
                        bool (*fn_close)(void *),
                        bool (*fn_write)(void *, void *, size_t)) {
  // Interpret connection data as upload state structure
  struct upload_state *us = (struct upload_state *) c->data;

  // Extract target file name from URI path
  char path[MAX_FILE_NAME];
  get_file_name_from_uri(hm->uri, path, sizeof(path));

  // Clear any existing upload state for a clean start
  memset(us, 0, sizeof(*us));

  // Attempt to open destination (file, flash memory, etc.) for upload
  us->fp = fn_open(path, hm->body.len);
  MG_DEBUG(("file: [%s] size: %lu fp: %p", path, hm->body.len, us->fp));

  // Mark the connection as being in "upload" mode
  us->marker = 'U';

  if (us->fp == NULL) {
    // Failed to open the destination — respond with error and prepare to close
    mg_http_reply(c, 400, JSON_HEADERS, "File open error\n");
    c->is_draining = 1;
  } else {
    // Store upload metadata and user-supplied file I/O callbacks
    us->expected = hm->body.len;
    us->fn_close = fn_close;
    us->fn_write = fn_write;

    // Remove the HTTP request headers from the input buffer
    mg_iobuf_del(&c->recv, 0, hm->head.len);

    // Swap the connection handler to process raw body data incrementally
    c->fn = upload_handler;

    // Disable default HTTP parsing to fully take over stream handling
    c->pfn = NULL;

    // Immediately invoke the upload handler with any pre-buffered data
    mg_call(c, MG_EV_READ, &c->recv.len);
  }
}




void handle_uploads(struct mg_connection *c, int ev, void *ev_data) {
  struct upload_state *us = (struct upload_state *) c->data;
  struct mg_http_message *hm = (struct mg_http_message *) ev_data;

  if (ev == MG_EV_HTTP_HDRS && us->marker == 0 &&
      mg_strcmp(hm->method, mg_str("POST")) == 0) {
      // Match against your OTA upload endpoint
    if (mg_match(hm->uri, mg_str("/api/firmware_update/*"), NULL)) {
      // auth user by cookie
      struct user *current_user = authenticate(hm);
      if (current_user == NULL){
        mg_http_reply(c, 404, "", "user not found\n");
      }
      else{
        prep_upload(c, hm,
          begin_firmware_update,
          end_firmware_update,
          write_firmware_update);
      }
    }
  }
}
