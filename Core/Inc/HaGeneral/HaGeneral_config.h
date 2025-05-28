/**
 * @file    HaGeneral_config.h
 * @author  yam
 * @date    Apr 21, 2025
 * @brief   Global configuration file for HaGeneral firmware.
 *
 * This file contains default settings for various subsystems of the firmware,
 * including network configuration, serial communication configuration(e.g., I2C), 
 * peripherals configuration and more...
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
 * @attention
 * I2C4_MODE        ->      I2C_MODE_STD || I2C_MODE_FAST || I2C_MODE_FAST_PLUS
 * I2C4_BASE_SLAVE_ADDR  ->      7-BIT ADDR SHIFTED TO THE LEFT
 * I2C4_OP_DELAY    ->      UNITS == ms
 */
#define I2C4_MODE I2C_MODE_FAST
#define I2C4_BASE_SLAVE_ADDR (0x50 << 1) // EEPROM ADDR
#define I2C4_OP_DELAY 50

// SERIAL COMM *********************************************************************************


// PERIPHERALS *********************************************************************************

// ADC
/** ADC3
 * Features:
 * - 12-bit resolution.
 * - Pin mappings:
 *   - PC3_C (ADC3_INP1): Used for single-ended input.
 *   - PC2_C (ADC3_INN1): Used as negative input for differential mode.
 * - ADC reference voltage: 3.3V.
 *
 * Notes:
 * - The analog watchdog thresholds must be within the valid range of 0 to 2^12-1 (0–4095).
 * - The DMA buffer is placed in SRAM4 (16KB) for BDMA access.
 * - Ensure DMA buffer size does not exceed SRAM4 capacity.
 * - This configuration supports flexible operation modes (DMA or polling).
 *
 * Configuration Macros:
 * - ADC3_HANDLE_STATUS:                                HANDLE_ON or HANDLE_OFF.
 * - ADC3_SINGLE_OR_DOUBLE_ENDED:                       ADC_SINGLE_ENDED or ADC_DIFFERENTIAL_ENDED.
 * - ADC3_ANALOG_WATCHDOG:                              HANDLE_ON or HANDLE_OFF.
 * - ADC3_ANALOG_WATCHDOG_HIGH_THRESHOLD:               0 - 4095.
 * - ADC3_ANALOG_WATCHDOG_LOW_THRESHOLD:                0 - 4095.
 * - ADC3_POLLING_OR_DMA_MODE:                          ADC_POLLING_MODE or ADC_DMA_MODE.
 * - ADC3_DMA_BUFFER_SIZE: DMA buffer size for BDMA.    1 - 16 * 1024
 *
 * Example:
 * @code
 * #define ADC3_HANDLE_STATUS                          HANDLE_ON
 * #define ADC3_SINGLE_OR_DOUBLE_ENDED                 ADC_SINGLE_ENDED
 * #define ADC3_ANALOG_WATCHDOG                        HANDLE_ON
 * #define ADC3_ANALOG_WATCHDOG_HIGH_THRESHOLD         3000
 * #define ADC3_ANALOG_WATCHDOG_LOW_THRESHOLD          0
 * #define ADC3_POLLING_OR_DMA_MODE                    ADC_DMA_MODE
 * #define ADC3_DMA_BUFFER_SIZE                        256
 * @endcode
 *
 * @warning Ensure correct settings for analog watchdog thresholds and DMA buffer size.
 * @warning Use SRAM4 for DMA buffer as BDMA is only connected to this region.
 */
#define ADC3_HANDLE_STATUS                          HANDLE_ON        // HANDLE_ON            OR        HANDLE_OFF
#define ADC3_SINGLE_OR_DOUBLE_ENDED                 ADC_SINGLE_ENDED // ADC_SINGLE_ENDED     OR        ADC_DIFFERENTIAL_ENDED

#define ADC3_ANALOG_WATCHDOG                        HANDLE_ON        // HANDLE_ON            OR        HANDLE_OFF
// ANALOG WATCHDOG SETTINGS
#define ADC3_ANALOG_WATCHDOG_HIGH_THRESHOLD         3000             // value should be in between 0 - 4095 (2**12 - 1)
#define ADC3_ANALOG_WATCHDOG_LOW_THRESHOLD          0                // value should be in between 0 - 4095 (2**12 - 1)

#define ADC3_POLLING_OR_DMA_MODE                    ADC_DMA_MODE     // ADC_POLLING_MODE     OR        ADC_DMA_MODE
// DMA SETTINGS
#define ADC3_DMA_BUFFER_SIZE                        256              // value should be in between 0 - 16K (sram4 is size 16KB)



/** ADC2
 * @brief 
 * 
 */
#define ADC2_HANDLE_STATUS                          HANDLE_ON        // HANDLE_ON            OR        HANDLE_OFF
#define ADC2_SINGLE_OR_DOUBLE_ENDED                 ADC_DIFFERENTIAL_ENDED // ADC_SINGLE_ENDED     OR        ADC_DIFFERENTIAL_ENDED

#define ADC2_ANALOG_WATCHDOG                        HANDLE_OFF        // HANDLE_ON            OR        HANDLE_OFF
// ANALOG WATCHDOG SETTINGS
#define ADC2_ANALOG_WATCHDOG_HIGH_THRESHOLD         3000             // value should be in between 0 - 4095 (2**12 - 1)
#define ADC2_ANALOG_WATCHDOG_LOW_THRESHOLD          0                // value should be in between 0 - 4095 (2**12 - 1)

#define ADC2_POLLING_OR_DMA_MODE                    ADC_POLLING_MODE     // ADC_POLLING_MODE     OR        ADC_DMA_MODE
// DMA SETTINGS
#define ADC2_DMA_BUFFER_SIZE                        256              // value should be in between 0 - 16K (sram4 is size 16KB)




#define ADC1_HANDLE_STATUS              HANDLE_OFF


#endif /* INC_HAGENERAL_CONFIG_H_ */
