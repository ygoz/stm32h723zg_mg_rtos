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
 * This handler is responsible for receiving firmware binary data in chunks
 * via HTTP POST requests. It manages the OTA (Over-The-Air) update process,
 * including:
 * 
 * - Parsing the `name`, `offset`, and `total` parameters from the HTTP query.
 * - Initializing the OTA process when `offset == 0`.
 * - Writing firmware chunks to flash at address `0x08080000 + offset`.
 * - Finalizing the OTA process when the last empty chunk is received.
 * - MCU will reboot when the update is done
 *
 * Expected client-side upload process:
 * 1. Start with offset = 0 and total = full size of the .bin file.
 * 2. Send chunks of data (e.g., 1KB) with correct `offset` values.
 * 3. Finish with an empty chunk to signal OTA completion.
 *
 * Requirements:
 * - The total size must fit within 3 flash sectors (384KB).
 * - The OTA process uses sectors 4–6 (new firmware) and sector 7 (temporary swap).
 * 
 * @param[in] c  Mongoose connection handle.
 * @param[in] hm Mongoose HTTP message containing the body and query parameters.
 */
void handle_firmware_upload(struct mg_connection *c, struct mg_http_message *hm);

#endif /* INC_HTTP_FOTA_FIRMWARE_UPDATE_H_ */