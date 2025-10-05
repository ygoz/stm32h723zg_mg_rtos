
#include <string.h>
#include "http/routers/post_router.h"
#include "mongoose.h"

#include "http/FOTA/firmware_update.h"
#include "string.h"
#include "flash/internal_flash.h"
#include "http/settings/network.h"
#include "HaGeneral/HaGeneral_config.h"
#include "serial_comm/i2c/hi2c4.h"
#include "serial_comm/i2c/hi2c1.h"
#include "peripherals/dac/hdac1.h"
#include "http/routers/main_router.h"


void POST_requests_router(struct mg_connection *c, struct mg_http_message *hm){
	char response[256] = {0};

	if (mg_match(hm->uri, mg_str("/api/network_settings"), NULL)) {

		network_settings new_settings;  // put in the begining
		new_settings.is_initialized = 0xDEADBEEF; // magic word to initialize the settings in the flash

		// Set defaults first
		strncpy(new_settings.ip, HAGENRAL_DEFAULT_IP, sizeof(new_settings.ip));
		strncpy(new_settings.gateway, HAGENRAL_DEFAULT_GATEWAY, sizeof(new_settings.gateway));
		strncpy(new_settings.netmask, HAGENRAL_DEFAULT_SUBNETMASK, sizeof(new_settings.netmask));
		new_settings.dhcp = false;
		
		struct mg_str json = hm->body;

		// Extract "ip_address"
		char *ip = mg_json_get_str(json, "$.ip_address");
		if (ip != NULL && is_valid_addr(ip)) {
		strncpy(new_settings.ip, ip, sizeof(new_settings.ip));
		free(ip);
		}

		// Extract "gw_address"
		char *gw = mg_json_get_str(json, "$.gw_address");
		if (gw != NULL && is_valid_addr(gw)) {
		strncpy(new_settings.gateway, gw, sizeof(new_settings.gateway));
		free(gw);
		}

		// Extract "netmask"
		char *mask = mg_json_get_str(json, "$.netmask");
		if (mask != NULL && is_valid_addr(mask)) {
		strncpy(new_settings.netmask, mask, sizeof(new_settings.netmask));
		free(mask);
		}

		// Extract "dhcp" boolean
		// bool dhcp_value;
		// if (mg_json_get_bool(json, "$.dhcp", &dhcp_value)) {
		// new_settings.dhcp = dhcp_value;
		// }

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


	else if (mg_match(hm->uri, mg_str("/api/eeprom/write"), NULL)) {
		char addr_buf[8], slave_buf[8], text_buf[256];
	
		// Extract query parameters: addr (memory), slave (device), text (data)
		mg_http_get_var(&hm->query, "addr", addr_buf, sizeof(addr_buf));
		mg_http_get_var(&hm->query, "slave", slave_buf, sizeof(slave_buf));
		mg_http_get_var(&hm->query, "text", text_buf, sizeof(text_buf));
	
		// Convert address parameters
		uint16_t mem_addr = (uint16_t)strtol(addr_buf, NULL, 0);
		uint16_t slave_addr = (uint16_t)strtol(slave_buf, NULL, 0);
	
		// Prepare data packet
		I2C_packet packet = {
			.data = (uint8_t *)text_buf,
			.size = (uint16_t)strlen(text_buf)
		};
	
		// Write to EEPROM
		// HAL_StatusTypeDef status = I2C4_mem_write(mem_addr, packet, slave_addr);
		HAL_StatusTypeDef status = I2C1_mem_write(mem_addr, packet, slave_addr);
	
		if (status == HAL_OK) {
			mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{\"success\":true}\n");
		} else {
			mg_http_reply(c, 500, "Content-Type: application/json\r\n",
				"{\"success\":false,\"status\":%d}\n", status);
		}
	}

	else if (mg_match(hm->uri, mg_str("/api/leds"), NULL)) {
		bool val;

		// Red LED
		if (mg_json_get_bool(hm->body, "$.red", &val)) {
			HAL_GPIO_WritePin(LED_RED_D1_GPIO_Port, LED_RED_D1_Pin, val ? GPIO_PIN_RESET : GPIO_PIN_SET);
		}

		// Green LED
		if (mg_json_get_bool(hm->body, "$.green", &val)) {
			HAL_GPIO_WritePin(LED_GREEN_D1_GPIO_Port, LED_GREEN_D1_Pin, val ? GPIO_PIN_RESET : GPIO_PIN_SET);
		}

		// Blue LED
		if (mg_json_get_bool(hm->body, "$.blue", &val)) {
			HAL_GPIO_WritePin(LED_BLUE_D1_GPIO_Port, LED_BLUE_D1_Pin, val ? GPIO_PIN_RESET : GPIO_PIN_SET);
		}

		s_device_change_version++;

		mg_http_reply(c, 200, "", "{\"status\": \"ok\"}");
	}

	else if (mg_match(hm->uri, mg_str("/api/hil"), NULL)) {

		mg_http_reply(c, 200, "Content-Type: application/json\r\n", "");
	}

	else if (mg_match(hm->uri, mg_str("/api/periph/dac1"), NULL)) {
	
		double dac_channel, dac_value;

		bool ok1 = mg_json_get_num(hm->body, "$.channel", &dac_channel);
		bool ok2 = mg_json_get_num(hm->body, "$.value", &dac_value);


		if (!ok1 || !ok2) {
			mg_http_reply(c, 422, "Content-Type: application/json\r\n",
				"{\"success\":false,\"error\":\"Missing or invalid parameter%s: %s%s\"}\n",
				(!ok1 && !ok2) ? "s" : "",
				!ok1 ? "channel" : "",
				(!ok1 && !ok2) ? " and value" : (!ok2 ? "value" : "")
			);
			return;
		}

		HAL_StatusTypeDef status = dac1_set_value((uint32_t)dac_value, (uint32_t)dac_channel);
	
		if (status == HAL_OK) {
			mg_http_reply(c, 200, "Content-Type: application/json\r\n", "{\"success\":true}\n");
		} else {
			mg_http_reply(c, 500, "Content-Type: application/json\r\n",
				"{\"success\":false,\"status\":%d}\n", status);
		}
	}
	

	else{
		mg_http_reply(c, 404, "", "this jjdcjcjdcjnj else post router\r\n");
	}

}
