/**
 * @file    HaGeneral_config.h
 * @author  yam
 * @date    Apr 21, 2025
 * @brief   Global configuration file for HaGeneral firmware.
 *
 * This file contains default settings for various subsystems of the firmware,
 * including network configuration and serial communication (e.g., I2C).
 * 
 * The values defined here serve as fallbacks or compile-time constants,
 * and are typically used during the first boot of the MCU after flashing,
 * before dynamic runtime configuration is loaded or saved.
 *
 * @note Modify with caution—changes affect the behavior of the system globally.
 *       Ensure consistency across modules that rely on these definitions.
 */

#ifndef INC_HAGENERAL_CONFIG_H_
#define INC_HAGENERAL_CONFIG_H_





// SETTINGS ************************************************************************************

/** DEFAULT NETWORK SETTINGS
 * @brief   These definitions specify the default network parameters 
 *          in case no network settings were yet set after the MCU has been flashed. 
 *          Typically used during initial setup.
 *
 * These settings provide static IP configuration fallback values used 
 * by the system before any user-defined configuration is stored or loaded.
 *
 * @note Ensure these values match your local network configuration to avoid conflicts.
 */
#define HAGENRAL_DEFAULT_SUBNETMASK		"255.255.255.0"
#define HAGENRAL_DEFAULT_GATEWAY		"192.168.1.1"
#define HAGENRAL_DEFAULT_IP				"192.168.1.10"
#define HAGENRAL_DEFAULT_DHCP			false

// SETTINGS ************************************************************************************


// SERIAL COMM *********************************************************************************

// I2C
/** I2C4
 * @brief   Configuration for I2C4 peripheral using blocking I2C HAL calls.
 * 
 * This configuration is used for initializing and handling I2C4 transactions
 * in blocking mode, which is suitable for simple peripheral communication such as EEPROM.
 * 
 * @note    I2C4 is mapped to the following GPIO pins:
 *          - SCL: PD12
 *          - SDA: PF15
 * 
 *          Ensure appropriate pull-up resistors are present on both lines,
 *          and GPIO alternate functions are correctly configured for I2C operation.
 * 
 *          The settings below (timing, addressing mode, filters, etc.) are applied
 *          in @file serial_comm/i2c/hi2c4.c.
 */
#define I2C4_MODE I2C4_MODE_FAST
#define BASE_SLAVE_ADDR (0x50 << 1) // 7-BIT
#define I2C4_OP_DELAY 50 // in ms

// SERIAL COMM *********************************************************************************


#endif /* INC_HAGENERAL_CONFIG_H_ */
