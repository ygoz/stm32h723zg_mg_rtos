/*
 * firmware_update.h
 *
 *  Created on: Mar 18, 2025
 *      Author: yam
 */

#ifndef INC_HTTP_FOTA_FIRMWARE_UPDATE_H_
#define INC_HTTP_FOTA_FIRMWARE_UPDATE_H_

#include "mongoose.h"

/**
 * @brief Handle firmware upload over HTTP using Mongoose.
 *
 * This handler manages the Over-The-Air (OTA) firmware update process by
 * receiving and writing firmware binary data in chunks via HTTP POST requests.
 *
 * It performs the following steps:
 * - Parses `name`, `offset`, and `total` from the HTTP query string.
 * - Calls `mg_ota_begin()` to initialize OTA when `offset == 0`.
 * - Writes data chunks to flash at address `0x08080000 + offset` using `mg_ota_write()`.
 * - Calls `mg_ota_end()` after receiving the final empty chunk to finalize the update.
 *   On success, this rearranges flash sectors and triggers an MCU reboot to load the new firmware.
 *
 * Error Handling:
 * - Errors (e.g., missing parameters, OTA init failure, flash write failure) are
 *   reported to the client via HTTP 500 responses with explanatory messages.
 * - A successful write operation responds with HTTP 200 and a `"true\n"` message.
 *
 * Expected client-side upload flow:
 * 1. Start with `offset = 0` and `total =` full size of the `.bin` file.
 * 2. Send binary data in chunks (e.g., 1KB each) with correct `offset` values.
 * 3. Send an empty chunk to indicate completion.
 *
 * Flash Sector Usage:
 * - Sectors 0–3: Current firmware
 * - Sectors 4–6: New firmware (OTA update area)
 * - Sector 7: Temporary swap area used during finalization
 *
 * Requirements:
 * - Firmware size must not exceed 384KB (3 sectors of 128KB).
 *
 * @param[in] c  Mongoose connection handle.
 * @param[in] hm Mongoose HTTP message containing body and query parameters.
 */
void handle_firmware_upload(struct mg_connection *c, struct mg_http_message *hm);

#endif /* INC_HTTP_FOTA_FIRMWARE_UPDATE_H_ */