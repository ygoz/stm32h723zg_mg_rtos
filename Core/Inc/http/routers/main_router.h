/*
 * request_handler.h
 *
 *  Created on: Feb 13, 2025
 *      Author: yam
 */

#ifndef INC_HTTP_ROUTERS_MAIN_ROUTER_H_
#define INC_HTTP_ROUTERS_MAIN_ROUTER_H_



#include "mongoose.h"

static const uint64_t s_timeout_ms = 1500;  // Connect timeout in milliseconds

static void handle_http_request(struct mg_connection *c, void *ev_data);
void event_handler(struct mg_connection *c, int ev, void *ev_data);

#endif /* INC_HTTP_MAIN_ROUTER_H_ */
