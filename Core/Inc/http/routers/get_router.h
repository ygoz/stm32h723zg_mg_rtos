/*
 * get_router.h
 *
 *  Created on: Feb 13, 2025
 *      Author: yam
 */



#ifndef INC_HTTP_ROUTERS_GET_ROUTER_H_
#define INC_HTTP_ROUTERS_GET_ROUTER_H_

//#include "lwip/api.h" // For netconn functions
#include "mongoose.h"


/**
 * @brief Handles all incoming HTTP GET requests based on their URI.
 *
 * This function is the router for GET requests in the embedded web server.
 * It matches the request URI against predefined endpoints and performs the
 * appropriate action, such as toggling a GPIO, reading flash settings,
 * or returning system memory information.
 *
 * @param c   Pointer to the current Mongoose connection (`struct mg_connection`).
 * @param hm  Pointer to the parsed HTTP request message (`struct mg_http_message`).
 *
 * Supported endpoints:
 * - **`/api/ping`**: Responds with `"GET ping"` to test connectivity.
 * - **`/api/led/get`**: Returns the current state of the green LED (GPIO read).
 * - **`/api/led/toggle`**: Toggles the green LED and returns `"true"`.
 * - **`/flash/settings/get`**: Reads network settings from flash.
 *   - If settings are not initialized (`is_initialized != 0xDEADBEEF`), responds with HTTP 400.
 *   - If initialized, responds with IP, Gateway, Netmask, and DHCP status.
 * - **`/mem/ram/get`**: Returns available and total FreeRTOS heap memory in JSON format.
 * - **Fallback**: Serves static files from `/web_root` using packed file system if no route matches.
 *
 * @note The function uses `mg_match` for URI comparison and `mg_http_reply` to send HTTP responses.
 *
 * @see mg_http_message
 * @see mg_http_reply
 * @see mg_http_serve_dir
 */
void GET_requests_router(struct mg_connection *c, struct mg_http_message *hm);

#endif /* INC_HTTP_ROUTERS_GET_ROUTER_H_ */
