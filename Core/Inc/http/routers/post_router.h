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

void POST_requests_router(struct mg_connection *c, struct mg_http_message *hm);

#endif /* INC_HTTP_ROUTERS_POST_ROUTER_H_ */
