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

/** I2C1
 * @brief   Configuration for I2C1 peripheral using blocking I2C HAL calls.
 * 
 * This configuration is used for initializing and handling I2C1 transactions
 * in blocking mode, which is suitable for simple peripheral communication such as EEPROM.
 * 
 * @note    I2C1 is mapped to the following GPIO pins:
 *          - SCL: PB6
 *          - SDA: PB7
 * 
 *          Ensure appropriate pull-up resistors are present on both lines,
 *          and GPIO alternate functions are correctly configured for I2C operation.
 * 
 *          The settings below (timing, addressing mode, filters, etc.) are applied
 *          in @file serial_comm/i2c/hi2c1.c.
 * @attention
 * I2C1_MODE             ->      I2C_MODE_STD || I2C_MODE_FAST || I2C_MODE_FAST_PLUS
 * I2C1_BASE_SLAVE_ADDR  ->      7-BIT ADDR SHIFTED TO THE LEFT
 * I2C1_OP_DELAY         ->      UNITS == ms
 */
#define I2C1_MODE I2C_MODE_FAST
#define I2C1_BASE_SLAVE_ADDR (0x50 << 1) // EEPROM ADDR
#define I2C1_OP_DELAY 50

// UART
/** UART10
 * @brief   Configuration for UART10 peripheral using interrupt-driven mode.
 * 
 * This configuration enables asynchronous serial communication via UART10
 * with **interrupts** for both RX and TX operations.
 * UART10 is suitable for communicating with serial peripherals or logging data.
 * 
 * @note    UART10 is mapped to the following GPIO pins:
 *          - TX: PE3
 *          - RX: PE2
 * 
 *          RX operates in **byte-by-byte interrupt mode**: an interrupt is triggered for each byte received.
 *          Reception continues until either `\r` or `\n` is received, at which point the received string
 *          is null-terminated.
 * 
 *          TX uses interrupt-based transmission for non-blocking communication.
 *          Both TX and RX rely on HAL drivers using `HAL_UART_Transmit_IT` and `HAL_UART_Receive_IT`.
 * 
 * @attention
 * UART10_HANDLE_STATUS   ->   UART_HANDLE_ON || UART_HANDLE_OFF  
 * UART10_RX_BUFFER_SIZE  ->   Must be > 0 and <= 1024  
 * UART10_BAUD_RATE       ->   Typically 9600, 115200, etc. — ensure both ends of the communication match.
 */
#define UART10_HANDLE_STATUS        UART_HANDLE_OFF
#define UART10_RX_BUFFER_SIZE       128
#define UART10_BAUD_RATE            9600

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
#define ADC3_ANALOG_WATCHDOG_HIGH_THRESHOLD         4000             // value should be in between 0 - 4095 (2**12 - 1)
#define ADC3_ANALOG_WATCHDOG_LOW_THRESHOLD          0                // value should be in between 0 - 4095 (2**12 - 1)

#define ADC3_POLLING_OR_DMA_MODE                    ADC_DMA_MODE     // ADC_POLLING_MODE     OR        ADC_DMA_MODE
// DMA SETTINGS
#define ADC3_DMA_BUFFER_SIZE                        256              // value should be in between 0 - 16K (sram4 is size 16KB)



/** ADC2
 * Features:
 * - 16-bit resolution.
 * - Pin mappings:
 *   - PF13 (ADC2_INP1): Used for single-ended input.
 *   - PF14 (ADC2_INN1): Used as negative input for differential mode.
 * - ADC reference voltage: 3.3V.
 *
 * Notes:
 * - The analog watchdog thresholds must be within the valid range of 0 to 2^16-1 (0–65535).
 * - DMA mode is not supported; polling mode only.
 * - The ADC2 input mode (single-ended or differential) can be configured via macros.
 *
 * Configuration Macros:
 * - ADC2_HANDLE_STATUS:                                HANDLE_ON or HANDLE_OFF.
 * - ADC2_SINGLE_OR_DOUBLE_ENDED:                       ADC_SINGLE_ENDED or ADC_DIFFERENTIAL_ENDED.
 * - ADC2_ANALOG_WATCHDOG:                              HANDLE_ON or HANDLE_OFF.
 * - ADC2_ANALOG_WATCHDOG_HIGH_THRESHOLD:               0 - 65535.
 * - ADC2_ANALOG_WATCHDOG_LOW_THRESHOLD:                0 - 65535.
 *
 * Example:
 * @code
 * #define ADC2_HANDLE_STATUS                          HANDLE_ON
 * #define ADC2_SINGLE_OR_DOUBLE_ENDED                 ADC_SINGLE_ENDED
 * #define ADC2_ANALOG_WATCHDOG                        HANDLE_ON
 * #define ADC2_ANALOG_WATCHDOG_HIGH_THRESHOLD         50000
 * #define ADC2_ANALOG_WATCHDOG_LOW_THRESHOLD          0
 * @endcode
 *
 * @warning DMA mode is not supported for ADC2. Use polling mode only.
 * @warning Ensure correct settings for analog watchdog thresholds.
 */
#define ADC2_HANDLE_STATUS                          HANDLE_ON        // HANDLE_ON            OR        HANDLE_OFF
#define ADC2_SINGLE_OR_DOUBLE_ENDED                 ADC_SINGLE_ENDED // ADC_SINGLE_ENDED     OR        ADC_DIFFERENTIAL_ENDED

#define ADC2_ANALOG_WATCHDOG                        HANDLE_OFF        // HANDLE_ON            OR        HANDLE_OFF
// ANALOG WATCHDOG SETTINGS
#define ADC2_ANALOG_WATCHDOG_HIGH_THRESHOLD         50000             // value should be in between 0 - 64 * 1024 - 1 (2**16 - 1)
#define ADC2_ANALOG_WATCHDOG_LOW_THRESHOLD          0                // value should be in between 0 - 64 * 1024 - 1 (2**16 - 1)



/** ADC1
 * Features:
 * - 16-bit resolution.
 * - Pin mappings:
 *     - PF11 (ADC3_INP1): Used for single-ended input.
 *     - PF12 (ADC3_INN1): Used as negative input for differential mode.
 * - ADC reference voltage: 3.3V.
 *
 * Notes:
 * - The analog watchdog thresholds must be within the valid range of 0 to 2^16-1 (0–65535).
 * - DMA mode is not supported; polling mode only.
 * - The ADC2 input mode (single-ended or differential) can be configured via macros.
 *
 * Configuration Macros:
 * - ADC1_HANDLE_STATUS:                                HANDLE_ON or HANDLE_OFF.
 * - ADC1_SINGLE_OR_DOUBLE_ENDED:                       ADC_SINGLE_ENDED or ADC_DIFFERENTIAL_ENDED.
 * - ADC1_ANALOG_WATCHDOG:                              HANDLE_ON or HANDLE_OFF.
 * - ADC1_ANALOG_WATCHDOG_HIGH_THRESHOLD:               0 - 65535.
 * - ADC1_ANALOG_WATCHDOG_LOW_THRESHOLD:                0 - 65535.
 * 
 * @warning DMA mode is not supported for ADC2. Use polling mode only.
 * @warning Ensure correct settings for analog watchdog thresholds.
 */
#define ADC1_HANDLE_STATUS                          HANDLE_ON        // HANDLE_ON            OR        HANDLE_OFF
#define ADC1_SINGLE_OR_DOUBLE_ENDED                 ADC_SINGLE_ENDED // ADC_SINGLE_ENDED     OR        ADC_DIFFERENTIAL_ENDED

#define ADC1_ANALOG_WATCHDOG                        HANDLE_ON        // HANDLE_ON            OR        HANDLE_OFF
// ANALOG WATCHDOG SETTINGS
#define ADC1_ANALOG_WATCHDOG_HIGH_THRESHOLD         50000             // value should be in between 0 - 64 * 1024 - 1 (2**16 - 1)
#define ADC1_ANALOG_WATCHDOG_LOW_THRESHOLD          0                // value should be in between 0 - 64 * 1024 - 1 (2**16 - 1)

// COMP
/**
 * COMP1
 * 
 * Features:
 * - Comparator peripheral with selectable operating modes:
 *     - COMP1_MODE_OFF: Comparator disabled, no power consumption.
 *     - COMP1_MODE_POLLING: Comparator output read by polling.
 *     - COMP1_MODE_IT: Comparator triggers interrupt on output edge.
 * - Hysteresis levels to reduce noise sensitivity:
 *     - COMP1_HYSTERESIS_NONE: No hysteresis.
 *     - COMP1_HYSTERESIS_LOW: Low hysteresis.
 *     - COMP1_HYSTERESIS_MEDIUM: Medium hysteresis.
 *     - COMP1_HYSTERESIS_HIGH: High hysteresis.
 * - Interrupt trigger modes:
 *     - COMP1_TRIGGER_MODE_RISING: Interrupt on rising output edge.
 *     - COMP1_TRIGGER_MODE_FALLING: Interrupt on falling output edge.
 *     - COMP1_TRIGGER_MODE_RISING_FALLING: Interrupt on both edges.
 * - Pin mappings:
 *     - PB0: Positive input (INP).
 *     - PB1: Negative input (INM).
 * 
 * Configuration macros (define these in HaGeneral_config.h):
 * - COMP1_MODE: Select the operating mode (default: COMP1_MODE_POLLING).
 * - COMP1_HYSTERESIS_MODE: Select hysteresis level (default: COMP1_HYSTERESIS_NONE).
 * - COMP1_TRIGGER_MODE: Select interrupt trigger mode (default: COMP1_TRIGGER_MODE_RISING_FALLING).
 * 
 * @warning Make sure only one COMP1_MODE is selected.
 */
#define COMP1_MODE                                  COMP1_MODE_OFF
#define COMP1_HYSTERESIS_MODE                       COMP1_HYSTERESIS_NONE
#define COMP1_TRIGGER_MODE                          COMP1_TRIGGER_MODE_RISING_FALLING

#endif /* INC_HAGENERAL_CONFIG_H_ */
