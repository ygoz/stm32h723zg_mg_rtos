/*
 * network.h
 *
 *  Created on: Apr 20, 2025
 *      Author: yam
 */

#ifndef INC_HTTP_SETTINGS_NETWORK_H_
#define INC_HTTP_SETTINGS_NETWORK_H_





#define NETWORK_SETTINGS_ADDR 0x080A0000
//#define GENERAL_SETTINGS_ADDR (NETWORK_SETTINGS_ADDR + sizeof(network_settings))



#include <stdbool.h>
#include "mongoose.h"
#include "HaGeneral/HaGeneral_config.h"







// use __packed?
typedef struct {
  char netmask[16];
  char gateway[16];
  char ip[16];
  bool dhcp;
  uint32_t is_initialized;  // Indicates whether settings were previously written ****magic != 0xDEADBEEF
} network_settings;





// put documentation here
void network_settings_init(network_settings *settings);
void get_network_settings(network_settings *data);
void set_network_settings(network_settings *data, struct mg_mgr *mgr);
void set_ip_configurations(void *arg);
void change_driver_settings(network_settings *settings, struct mg_mgr *mgr);







#endif /* INC_HTTP_SETTINGS_NETWORK_H_ */
