/*
 * firmware_update.h
 *
 *  Created on: Mar 18, 2025
 *      Author: yam
 */

#ifndef INC_HTTP_FOTA_FIRMWARE_UPDATE_H_
#define INC_HTTP_FOTA_FIRMWARE_UPDATE_H_

#include "mongoose.h"
#include "http/routers/main_router.h"








/**
 * @brief State structure for tracking an ongoing file upload.
 *
 * This structure is stored in `c->data` during an HTTP POST-based upload
 * (e.g., OTA firmware update). It holds the file pointer, progress tracking,
 * and function callbacks required to process and complete the upload.
 *
 * The `marker` field is used to flag that a given connection is currently
 * handling an upload. This helps the upload handler differentiate between
 * normal and upload-specific traffic.
 */
struct upload_state {
  char marker;               // Tells that we're a file upload connection
  size_t expected;           // POST data length, bytes
  size_t received;           // Already received bytes
  void *fp;                  // Opened file
  bool (*fn_close)(void *);  // Close function
  bool (*fn_write)(void *, void *, size_t);  // Write function
};



/**
 * @brief Handles OTA firmware upload requests over HTTP.
 *
 * This function should be called from within the main HTTP event handler.
 * It checks if the request is a POST to `/api/firmware_update/*`, performs
 * user authentication, and if successful, prepares the connection for receiving
 * a file upload (typically firmware data).
 *
 * @param c         Pointer to the Mongoose connection.
 * @param ev        Event type (e.g., MG_EV_HTTP_HDRS).
 * @param ev_data   Event data, expected to be a pointer to `struct mg_http_message`.
 */
void handle_uploads(struct mg_connection *c, int ev, void *ev_data);

#endif /* INC_HTTP_FOTA_FIRMWARE_UPDATE_H_ */