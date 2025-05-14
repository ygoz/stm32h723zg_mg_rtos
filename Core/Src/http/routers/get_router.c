#include <string.h>
#include "http/routers/get_router.h"
#include "mongoose.h"
#include "main.h"
#include "FreeRTOSConfig.h"

#include "flash/internal_flash.h"
#include "http/settings/network.h"
#include "string.h"





void GET_requests_router(struct mg_connection *c, struct mg_http_message *hm){

	char response[256] = {0};

	if (mg_match(hm->uri, mg_str("/api/ping"), NULL)) {
		mg_http_reply(c, 200, "", "GET ping\r\n");
		}

	else if (mg_match(hm->uri, mg_str("/api/led/get"), NULL)) {
	    mg_http_reply(c, 200, "", "%d\n", HAL_GPIO_ReadPin(GPIOB, LED_GREEN_Pin));
	    }

	else if (mg_match(hm->uri, mg_str("/api/led/toggle"), NULL)) {
		HAL_GPIO_TogglePin(GPIOB, LED_GREEN_Pin); // Can be different on your board
	    mg_http_reply(c, 200, "", "true\n");
	    }

	else if (mg_match(hm->uri, mg_str("/flash/settings/get"), NULL)){
		network_settings settings;
		if (get_network_settings(&settings)){ // Read settings from flash and convert to struct
			mg_http_reply(c, 500, "", "could not read settings from flash\r\n"); // Read failed
		}  
		else{
			if (settings.is_initialized != 0xDEADBEEF) { // check if settings were initialized
				mg_http_reply(c, 400, "", "error: settings were never initialized\r\n");
			} 
			else {
				// Settings are valid, return them
				snprintf(response, sizeof(response),
						 "ok settings were set:\r\n"
						 "IP: %s\r\n"
						 "Gateway: %s\r\n"
						 "Netmask: %s\r\n"
						 "DHCP: %s\r\n",
						 settings.ip,
						 settings.gateway,
						 settings.netmask,
						 settings.dhcp ? "Enabled" : "Disabled");
				mg_http_reply(c, 200, "", "%s", response);
			}
		}
	}

	else if (mg_match(hm->uri, mg_str("/mem/ram/get"), NULL)) {
	    uint32_t free_memory = xPortGetFreeHeapSize();  // Example: Get the available heap memory
	    uint32_t total_memory = configTOTAL_HEAP_SIZE;  // Total heap memory size from FreeRTOS config

	    // Format the response as JSON or plain text
	    snprintf(response, sizeof(response), "{\"free_memory\": %u, \"total_memory\": %u}\n", free_memory, total_memory);
	    mg_http_reply(c, 200, "", response);
	}

	else {
		struct mg_http_serve_opts opts = {.root_dir = "/web_root", .fs = &mg_fs_packed};
		mg_http_serve_dir(c, hm, &opts);
	}
}
