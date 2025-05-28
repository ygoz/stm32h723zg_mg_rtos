#include <string.h>
#include "http/routers/get_router.h"
#include "mongoose.h"
#include "main.h"
#include "FreeRTOSConfig.h"

#include "flash/internal_flash.h"
#include "http/settings/network.h"
#include "string.h"
#include "serial_comm/i2c/hi2c4.h"
#include "peripherals/adc/hadc3.h"





void GET_requests_router(struct mg_connection *c, struct mg_http_message *hm){

	char response[256] = {0};
	uint16_t http_status_code = 0;

	if (mg_match(hm->uri, mg_str("/api/ping"), NULL)) {
		mg_http_reply(c, 200, "", "GET ping\r\n");
		}

	else if (mg_match(hm->uri, mg_str("/api/led/green/get"), NULL)) {
	    mg_http_reply(c, 200, "", "%d\n", HAL_GPIO_ReadPin(GPIOB, LED_GREEN_Pin));
	    }

	else if (mg_match(hm->uri, mg_str("/api/led/green/toggle"), NULL)) {
		HAL_GPIO_TogglePin(GPIOB, LED_GREEN_Pin); // Can be different on your board
	    mg_http_reply(c, 200, "", "true\n");
	    }


	else if (mg_match(hm->uri, mg_str("/api/periph/adc#"), NULL)) {
		uint16_t adc_value = 0;

		if (hm->uri.len != 16) {
            mg_http_reply(c, 400, "", "Invalid URI length: %d\n", hm->uri.len);
            return;
        }

        // Extract and cast ADC number (last character) to int
        char adc_num_char = hm->uri.buf[hm->uri.len - 1];
		uint8_t adc_num = adc_num_char - '0';

        switch (adc_num) {
			case 1:
				snprintf(response, sizeof(response), "ADC%d not yet supported\n", adc_num);
				http_status_code = 404;
				break;
			case 2:
			#if ADC2_HANDLE_STATUS == HANDLE_OFF
				snprintf(response, sizeof(response), "adc2 handle is off");
				http_status_code = 400;

			#elif ADC2_HANDLE_STATUS == HANDLE_ON
				#if ADC2_POLLING_OR_DMA_MODE == ADC_DMA_MODE

					uint16_t* my_buffer = adc2_get_value();
					snprintf(response, sizeof(response), "adc2 value : %u, %u\n", my_buffer[0], my_buffer[1]);
					http_status_code = 200;

				#elif ADC2_POLLING_OR_DMA_MODE == ADC_POLLING_MODE

					
					if (adc2_get_value(&adc_value) == HAL_OK) {
						snprintf(response, sizeof(response), "adc2 value : %u\n", adc_value);
						http_status_code = 200;
					} else {
						snprintf(response, sizeof(response), "adc2 read failed\n");
						http_status_code = 500;
					}
		
				#endif
			#endif
				break;

			case 3:
			#if ADC3_HANDLE_STATUS == HANDLE_OFF
				snprintf(response, sizeof(response), "adc3 handle is off");
				http_status_code = 400;

			#elif ADC3_HANDLE_STATUS == HANDLE_ON
				#if ADC3_POLLING_OR_DMA_MODE == ADC_DMA_MODE

					uint16_t* my_buffer = adc3_get_value();
					snprintf(response, sizeof(response), "adc3 value : %u, %u\n", my_buffer[0], my_buffer[1]);
					http_status_code = 200;

				#elif ADC3_POLLING_OR_DMA_MODE == ADC_POLLING_MODE

					
					if (adc3_get_value(&adc_value) == HAL_OK) {
						snprintf(response, sizeof(response), "adc3 value : %u\n", adc_value);
						http_status_code = 200;
					} else {
						snprintf(response, sizeof(response), "adc3 read failed\n");
						http_status_code = 500;
					}
				
				#endif
			#endif
				break;
			default:
				snprintf(response, sizeof(response), "adc%d not supported\n", adc_num);
				http_status_code = 404;
				break;
		}
		mg_http_reply(c, http_status_code, "", response);
	}


	else if (mg_match(hm->uri, mg_str("/api/eeprom/read"), NULL)) {
		char addr_buf[8], slave_buf[8], size_buf[8];

		// Extract query parameters: addr (memory), slave (device), size (bytes)
		mg_http_get_var(&hm->query, "addr", addr_buf, sizeof(addr_buf));
		mg_http_get_var(&hm->query, "slave", slave_buf, sizeof(slave_buf));
		mg_http_get_var(&hm->query, "size", size_buf, sizeof(size_buf));
	
		// Convert to integers
		uint16_t mem_addr = (uint16_t)strtol(addr_buf, NULL, 0);
		uint16_t slave_addr = (uint16_t)strtol(slave_buf, NULL, 0);
		uint16_t size = (uint16_t)strtol(size_buf, NULL, 0);
	
		// Read EEPROM
		static uint8_t buffer[256];  // Max size you expect to read
		if (size > sizeof(buffer)) size = sizeof(buffer);  // Safety cap
	  
		I2C_packet packet = {
			.data = buffer,
			.size = size
		};

		HAL_StatusTypeDef status = I2C4_mem_read(mem_addr, packet, slave_addr);
		if (status == HAL_OK) {
			mg_http_reply(c, 200, "Content-Type: text/plain\r\n", "%.*s", packet.size, (char *)packet.data);
		} else {
			mg_http_reply(c, 500, "Content-Type: application/json\r\n",
				"{\"success\":false,\"status\":%d}\n", status);
		}
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
	    snprintf(response, sizeof(response), "{\"free_memory\": %lu, \"total_memory\": %lu}\n", free_memory, total_memory);
	    mg_http_reply(c, 200, "", response);
	}

	else {
		struct mg_http_serve_opts opts = {.root_dir = "/web_root", .fs = &mg_fs_packed};
		mg_http_serve_dir(c, hm, &opts);
	}
}
