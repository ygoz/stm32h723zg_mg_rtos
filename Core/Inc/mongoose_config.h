/*
 * mongoose_config.h
 *
 *  Created on: Mar 30, 2025
 *      Author: yam
 */

#ifndef INC_MONGOOSE_CONFIG_H_
#define INC_MONGOOSE_CONFIG_H_

// #pragma once

// OS
//#define MG_ARCH MG_ARCH_NEWLIB
#define MG_ARCH MG_ARCH_FREERTOS
#define MG_ENABLE_CUSTOM_RANDOM 1

#define MG_IRAM __attribute__((section(".RamFunc")))

// For all ARM GCC based environments
#define MG_ENABLE_TCPIP 1          // Enables built-in TCP/IP stack
#define MG_ENABLE_CUSTOM_MILLIS 1  // We must implement mg_millis()
//#define MG_ENABLE_TCPIP_PRINT_DEBUG_STATS 1  // Enable debug stats log
#define MG_ENABLE_DRIVER_STM32H 1

// Uncomment the driver for your device
 #define MG_ENABLE_DRIVER_STM32H 1
// #define MG_ENABLE_DRIVER_STM32F 1


// Network settings
#define MG_TCPIP_IP MG_IPV4(192, 168, 1, 10)  // Default is 0.0.0.0 (DHCP)
#define MG_TCPIP_MASK MG_IPV4(255, 255, 255, 0)  // Default is 0.0.0.0 (DHCP)
#define MG_TCPIP_GW MG_IPV4(192, 168, 1, 1)  // Default is 0.0.0.0 (DHCP)


// File System
#define MG_ENABLE_PACKED_FS 1  // Enable "embedded", or packed, filesystem
#define MG_ENABLE_POSIX_FS 0   // Disable POSIX filesystem

// FOTA
#define MG_OTA MG_OTA_STM32H7
//#define MG_OTA MG_OTA_FLASH  // Enable the flash-based OTA implementation


#endif /* INC_MONGOOSE_CONFIG_H_ */
