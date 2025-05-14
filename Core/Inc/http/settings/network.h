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



static network_settings default_network_settings = {
  .netmask            = HAGENRAL_DEFAULT_SUBNETMASK,
  .gateway            = HAGENRAL_DEFAULT_GATEWAY,
  .ip                 = HAGENRAL_DEFAULT_IP,
  .dhcp               = HAGENRAL_DEFAULT_DHCP,
  .is_initialized 	= 0xDEADBEEF
};



/**
 * @brief Initializes a network_settings structure with default values.
 *
 * This function sets the given `network_settings` structure to a predefined
 * set of default values by copying from `default_network_settings`.
 *
 * It is typically used to initialize the structure before first use or to
 * reset it to a known default state.
 *
 * @param[out] settings Pointer to the `network_settings` structure to initialize.
 *
 * @note The `default_network_settings` must be defined elsewhere in the program.
 */
void network_settings_init(network_settings *settings);



/**
 * @brief Reads network settings from Flash memory.
 *
 * This function retrieves the previously stored network settings from Flash memory
 * (specifically from sector 5, starting at address `NETWORK_SETTINGS_ADDR`) and
 * copies them into the provided `network_settings` structure.
 *
 * It calculates the number of 32-bit words required to read the full structure
 * (accounting for any remainder bytes) and reads the data using `flash_read_data()`.
 *
 * @param[out] data Pointer to a `network_settings` structure where the read data will be stored.
 *
 * @return uint8_t Status code returned by `flash_read_data()`:
 *          - `0` on success.
 *          - Non-zero error code on failure (e.g., Flash read error).
 * @note The caller must ensure that `data` points to a valid memory location large enough
 *       to hold a `network_settings` structure.
 */
uint8_t get_network_settings(network_settings *data);



/**
 * @brief Writes network configuration settings to Flash memory and schedules IP reconfiguration.
 *
 * This function stores the provided `network_settings` structure into a predefined
 * Flash memory location (`NETWORK_SETTINGS_ADDR`). It calculates how many 32-bit words
 * are required for the write operation and uses `flash_write_data()` to perform the write.
 * After writing to Flash, it sets a non-blocking timer (500 ms delay) that triggers
 * the `set_ip_configurations()` function, allowing deferred reconfiguration of the IP settings.
 *
 * @param[in] data Pointer to a `network_settings` structure containing the new configuration.
 * @param[in] mgr Pointer to the Mongoose event manager (used to schedule the timer).
 *
 * @return uint8_t Status code returned by `flash_write_data()`:
 *         - `0` on success.
 *         - Non-zero error code on failure (e.g., Flash write error).
 *
 * @see flash_write_data
 * @see set_ip_configurations
 */
uint8_t set_network_settings(network_settings *data, struct mg_mgr *mgr);


/**
 * @brief Applies network settings by configuring the Ethernet driver.
 *
 * This function reads the saved network settings from Flash memory using `get_network_settings()`,
 * then checks whether the settings are valid by comparing the `is_initialized` field against
 * a known magic number (`0xDEADBEEF`).
 *
 * - If the settings are valid, it passes them to `change_driver_settings()` to apply the configuration.
 * - If the settings are uninitialized, it logs a message and applies the `default_network_settings`.
 *
 * The function is intended to be used as a callback, scheduled via `mg_timer_add`, typically
 * after writing settings to Flash or during initialization.
 *
 * @param[in] arg Pointer to the Mongoose manager (`struct mg_mgr *`) passed during the timer setup.
 *
 * @note This function assumes that the Flash memory has already been initialized and contains
 *       either valid or default settings. It does not perform Flash unlock/lock operations.
 *
 * @see get_network_settings
 * @see change_driver_settings
 * @see mg_timer_add
 */
void set_ip_configurations(void *arg);




// void change_driver_settings(network_settings *settings, struct mg_mgr *mgr);







#endif /* INC_HTTP_SETTINGS_NETWORK_H_ */
