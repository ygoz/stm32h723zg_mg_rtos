/*
 * request_handler.c
 *
 *  Created on: Feb 13, 2025
 *      Author: yam
 */


#include "http/routers/get_router.h"
#include "http/routers/post_router.h"

#include "mongoose.h"


// Function to handle the request based on the type
void handle_http_request(struct mg_connection *c, void *ev_data) {

	struct mg_http_message *hm = (struct mg_http_message *) ev_data;
//		  printf("HTTP Body body: %.*s\r\n", (int) hm->body.len, hm->body.ptr);

		    if (mg_strcmp(hm->method, mg_str("GET")) == 0) {
		        GET_requests_router(c, hm);
		    } else if (mg_strcmp(hm->method, mg_str("POST")) == 0) {
		        POST_requests_router(c, hm);
//		    } else if (mg_strcmp(hm->method, mg_str("PUT")) == 0) {
//		        PUT_requests_router(c);
//		    } else if (mg_strcmp(hm->method, mg_str("DELETE")) == 0) {
//		        DELETE_requests_router(c);
		    } else {
		        mg_http_reply(c, 405, "", "Method Not Allowed\r\n");
		    }
//
//		  mg_http_reply(c, 200, "", "ok shmoopoo4\r\n");
}
