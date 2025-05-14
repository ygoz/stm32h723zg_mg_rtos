
#include <string.h>
#include "http/routers/post_router.h"
#include "mongoose.h"

#include "http/FOTA/firmware_update.h"
#include "string.h"
#include "flash/internal_flash.h"
#include "http/settings/network.h"
#include "HaGeneral/HaGeneral_config.h"


void POST_requests_router(struct mg_connection *c, struct mg_http_message *hm){
	char response[256] = {0};

	if (mg_match(hm->uri, mg_str("/api/firmware/upload"), NULL)) {
	  // add non blocking timer and send http reply at mg_ota_end?
	  handle_firmware_upload(c, hm);
	}

	else if (mg_match(hm->uri, mg_str("/flash/settings/set"), NULL)) {

		network_settings new_settings;  // put in the begining
		new_settings.is_initialized = 0xDEADBEEF; // magic word to initialize the settings in the flash

		// Netmask
		if (mg_http_get_var(&hm->query, "netmask", new_settings.netmask, sizeof(new_settings.netmask)) <= 0) {
		    strncpy(new_settings.netmask, HAGENRAL_DEFAULT_SUBNETMASK, sizeof(new_settings.netmask));
		}
		// Gateway
		if (mg_http_get_var(&hm->query, "gateway", new_settings.gateway, sizeof(new_settings.gateway)) <= 0) {
		    strncpy(new_settings.gateway, HAGENRAL_DEFAULT_GATEWAY, sizeof(new_settings.gateway));
		}
		// IP
		if (mg_http_get_var(&hm->query, "ip", new_settings.ip, sizeof(new_settings.ip)) <= 0) {
		    strncpy(new_settings.ip, HAGENRAL_DEFAULT_IP, sizeof(new_settings.ip));
		}
		// DHCP
		mg_http_get_var(&hm->query, "dhcp", (char *)&new_settings.dhcp, sizeof(new_settings.dhcp));


		if (set_network_settings(&new_settings, c->mgr)){
			mg_http_reply(c, 500, "", "could not set network settings\r\n");
		}
		
		else {
			// Prepare response message
			
			snprintf(response, sizeof(response), "ok settings updated: Netmask=%s, Gateway=%s, IP=%s, DHCP=%s\r\n",
					new_settings.netmask, new_settings.gateway, new_settings.ip, new_settings.dhcp ? "enabled" : "disabled");

			// Send the HTTP response with the updated settings
			mg_http_reply(c, 200, "", "%s", response);
		}
	}

	else{
		mg_http_reply(c, 404, "", "this jjdcjcjdcjnj else post router\r\n");
	}

}
