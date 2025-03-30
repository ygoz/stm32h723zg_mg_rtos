/*
 * request_handler.h
 *
 *  Created on: Feb 13, 2025
 *      Author: yam
 */

#ifndef INC_HTTP_ROUTERS_MAIN_ROUTER_H_
#define INC_HTTP_ROUTERS_MAIN_ROUTER_H_



#include "mongoose.h"

void handle_http_request(struct mg_connection *c, void *ev_data);

#endif /* INC_HTTP_MAIN_ROUTER_H_ */
