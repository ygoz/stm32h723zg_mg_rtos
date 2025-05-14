/*
 * post_router.h
 *
 *  Created on: Feb 13, 2025
 *      Author: yam
 */

#ifndef INC_HTTP_ROUTERS_POST_ROUTER_H_
#define INC_HTTP_ROUTERS_POST_ROUTER_H_

//#include "lwip/api.h" // For netconn functions
#include "mongoose.h"


/**
 * @brief Handles all incoming HTTP POST requests based on their URI.
 *
 * This function routes HTTP POST requests to the appropriate handler
 * based on the request URI. It processes firmware uploads and updates
 * for network settings stored in internal Flash.
 *
 * @param c   Pointer to the current Mongoose connection (`struct mg_connection`).
 * @param hm  Pointer to the parsed HTTP request message (`struct mg_http_message`).
 *
 * Supported endpoints:
 * - **`/api/firmware/upload`**:
 *   - Triggers `handle_firmware_upload()` to process a firmware binary upload.
 *   - Firmware upload is limited to 384 KB (3 sectors of 128 KB) — typically for half of a 1 MB flash.
 *
 * - **`/flash/settings/set`**:
 *   - Parses query parameters (`netmask`, `gateway`, `ip`, `dhcp`) and populates a `network_settings` struct.
 *   - If a parameter is missing, a default value is assigned.
 *   - Sets `is_initialized = 0xDEADBEEF` to mark the flash as initialized.
 *   - Calls `set_network_settings()` to write the settings to Flash memory.
 *   - Replies with a confirmation string including the applied settings.
 *
 * - **Fallback**:
 *   - If no route matches, responds with a 404 Not Found error and a placeholder debug message.
 *
 *
 * @see handle_firmware_upload()
 * @see set_network_settings()
 * @see mg_http_get_var()
 * @see mg_http_reply()
 */
void POST_requests_router(struct mg_connection *c, struct mg_http_message *hm);

#endif /* INC_HTTP_ROUTERS_POST_ROUTER_H_ */
