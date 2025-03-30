/*
 * request_handler.c
 *
 *  Created on: Feb 13, 2025
 *      Author: yam
 */


#include "http/routers/get_router.h"
#include "http/routers/post_router.h"
#include "http/routers/main_router.h"

#include "mongoose.h"
#include "main.h"



void mg_random(void *buf, size_t len) {  // Use on-board RNG
  extern RNG_HandleTypeDef hrng;
  for (size_t n = 0; n < len; n += sizeof(uint32_t)) {
    uint32_t r;
    HAL_RNG_GenerateRandomNumber(&hrng, &r);
    memcpy((char *) buf + n, &r, n + sizeof(r) > len ? len - n : sizeof(r));
  }
}



uint64_t mg_millis(void) {
  return HAL_GetTick();
}


// Function to handle the request based on the type
static void handle_http_request(struct mg_connection *c, void *ev_data) {

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
uint8_t static inline numconns(struct mg_mgr *mgr) {
  int n = 0;
  for (struct mg_connection *t = mgr->conns; t != NULL; t = t->next) {
//	  printf("IP ADDR 192.168.1.3 at port: %d", t->loc.port);
	  n++;
  }
  return n;
}

void event_handler(struct mg_connection *c, int ev, void *ev_data) {
	  if (ev == MG_EV_HTTP_MSG) {
		  handle_http_request(c, ev_data);
	  }
	  if (ev == MG_EV_ACCEPT) {
		  uint8_t active_connections = numconns(c->mgr);
		  printf("Active TCP connections: %d\r\n", active_connections);
		  if (active_connections > 5) {
	        MG_ERROR(("Too many connections\r\n"));
	        c->is_closing = 1;
	//  	  mg_mgr_free(c->mgr);
	//  	  mg_mgr_init(c->mgr);
		  }
	  }
	  else if (ev == MG_EV_ERROR){
		  printf("yams print - MG_EV_ERROR /r/n");
	  }
	}




