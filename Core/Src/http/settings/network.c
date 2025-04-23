///*
// * network.c
// *
// *  Created on: Apr 20, 2025
// *      Author: yam
// */
//
//
// network.c
#include "http/settings/network.h"
#include "flash/internal_flash.h"
#include "string.h"
#include "mongoose.h"
#include "http/routers/main_router.h"


#define NETWORK_SETTINGS_ADDR 0x080A0000
#define GENERAL_SETTINGS_ADDR (NETWORK_SETTINGS_ADDR + sizeof(network_settings))



void get_network_settings(network_settings *data) {
    // change to lower case
	uint16_t numofwords = (sizeof(network_settings) / 4) + ((sizeof(network_settings) % 4) != 0);
	Flash_Read_Data(NETWORK_SETTINGS_ADDR, (uint32_t *)data, numofwords);
}



void set_ip_configurations(void *arg) {


	struct mg_mgr *mgr = (struct mg_mgr *)arg;

	network_settings settings;
	get_network_settings(&settings);


    if (settings.dhcp){
    	mgr->ifp->enable_dhcp_client = true;
    	memset(&mgr->ifp->ip, 0, sizeof(mgr->ifp->ip));        // mgr->ifp->ip = {0,0,0,0} ?
    	memset(&mgr->ifp->mask, 0, sizeof(mgr->ifp->mask));
    	memset(&mgr->ifp->gw, 0, sizeof(mgr->ifp->gw));
    } else {
        // Disable DHCP client
    	struct mg_addr new_addr;
        mgr->ifp->enable_dhcp_client = false;
        
        mg_aton(mg_str(settings.ip), &new_addr);
        memcpy(&mgr->ifp->ip, &new_addr.ip, sizeof(mgr->ifp->ip));
        
        mg_aton(mg_str(settings.gateway), &new_addr);
        memcpy(&mgr->ifp->gw, &new_addr.ip, sizeof(mgr->ifp->ip));

        mg_aton(mg_str(settings.netmask), &new_addr);
        memcpy(&mgr->ifp->mask, &new_addr.ip, sizeof(mgr->ifp->ip));
    }
    for (struct mg_connection *c = mgr->conns; c != NULL; c = c->next){
    	if (c->is_listening == 0) c->is_closing = 1;
    }
    mgr->ifp->state = MG_TCPIP_STATE_DOWN;

    // You now have both mgr and settings
}


void set_network_settings(network_settings *data, struct mg_mgr *mgr) {


    // Calculate how many 32-bit words are needed to store the struct
    uint16_t num_words = sizeof(network_settings) / 4 + (sizeof(network_settings) % 4 != 0);

    // Write the struct directly to flash
    Flash_Write_Data(NETWORK_SETTINGS_ADDR, (uint32_t *)data, num_words);
    mg_timer_add(mgr, 500, 0, set_ip_configurations,  mgr); // Non-blocking delay to func set_ip_configurations
}

