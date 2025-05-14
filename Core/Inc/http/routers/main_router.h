/*
 * request_handler.h
 *
 *  Created on: Feb 13, 2025
 *      Author: yam
 */

#ifndef INC_HTTP_ROUTERS_MAIN_ROUTER_H_
#define INC_HTTP_ROUTERS_MAIN_ROUTER_H_



#include "mongoose.h"

static const uint64_t s_timeout_ms = 1500;  // Connect timeout in milliseconds -- MG_EV_POLL



/**
 * @brief Handles incoming HTTP requests by routing based on the HTTP method.
 *
 * This function is called by the Mongoose event loop to handle an HTTP request.
 * It inspects the HTTP method (e.g., GET, POST) and dispatches the request to
 * the appropriate router function. If the method is not supported, it replies
 * with HTTP 405 (Method Not Allowed).
 *
 * @param c         Pointer to the current Mongoose connection (`mg_connection`).
 * @param ev_data   Pointer to event-specific data, expected to be of type
 *                  `struct mg_http_message *`.
 *
 * Supported HTTP methods:
 * - **GET**: Routed to `GET_requests_router()`
 * - **POST**: Routed to `POST_requests_router()`
 * - **Other**: Returns `405 Method Not Allowed`
 *
 * @note This function can be extended in the future to support PUT, DELETE, etc.
 *
 * @see GET_requests_router()
 * @see POST_requests_router()
 */
static void handle_http_request(struct mg_connection *c, void *ev_data);



/**
 * @brief Mongoose event handler callback for managing connection lifecycle.
 *
 * This function handles various Mongoose connection events such as incoming HTTP requests,
 * new TCP connections, errors, and timeouts. It ensures resource limits are respected
 * and helps prevent connection stalls.
 *
 * @param c         Pointer to the current `mg_connection` structure.
 * @param ev        Event type (`MG_EV_*`) being handled.
 * @param ev_data   Event-specific data (cast depending on `ev`).
 *
 * Supported events:
 * - MG_EV_HTTP_MSG:  Dispatches the incoming HTTP request to `handle_http_request`.
 * - MG_EV_ACCEPT:    Limits incoming connections to a maximum of 10. Closes excess.
 * - MG_EV_ERROR:     Logs internal Mongoose error messages.
 * - MG_EV_OPEN:      Initializes a timeout timer by storing a future timestamp in `c->data`.
 * - MG_EV_POLL:      Checks if the connection has timed out and closes it if necessary.
 *
 * @note This function assumes `c->data` is large enough to hold a `uint64_t` timeout value.
 *       Be sure to zero or allocate it appropriately elsewhere in the connection setup.
 *
 * @see handle_http_request()
 * @see mg_millis()
 */
void event_handler(struct mg_connection *c, int ev, void *ev_data);

#endif /* INC_HTTP_MAIN_ROUTER_H_ */
