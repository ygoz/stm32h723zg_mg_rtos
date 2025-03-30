
#include <string.h>
#include "http/routers/post_router.h"
#include "mongoose.h"

#include "http/FOTA/firmware_update.h"




void POST_requests_router(struct mg_connection *c, struct mg_http_message *hm){

	if (mg_match(hm->uri, mg_str("/api/firmware/upload"), NULL)) {
	  // MAX FIRMWARE SIZE IS 128KB * 3 --> (3 sectors --> 1MB / 2 - 1 sector)
	  handle_firmware_upload(c, hm);

	}
	else if (mg_match(hm->uri, mg_str("/api/firmware/commit"), NULL)) {
		// This call will reset the device
		mg_http_reply(c, 200, "", "ok post router\r\n");
		// add delay? - make sure reply goes through
//		mg_ota_commit();
//		mg_ota_boot();
		printf("should be commited");
	   }
	else{
		mg_http_reply(c, 404, "", "this jjdcjcjdcjnj else post router\r\n");
		}

		printf("abukjsfj\r\n");
//	}
}
