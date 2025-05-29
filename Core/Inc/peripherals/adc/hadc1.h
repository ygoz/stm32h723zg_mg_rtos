/**
 * @file hadc3.h
 * @author Yam Goz (you@domain.com)
 * @brief Header file for ADC1 handle configuration and pin mapping.
 * @version 0.1
 * @date 2025-05-21
 * 
 * @copyright Copyright (c) 2025
 * 
 * @details
 * This file provides the configuration for ADC1, including its resolution and pin assignments.
 * - 16-bit resolution
 * - Working modes: ADC_POLLING_MODE
 * - Pin mappings:
 *     - PF11 (ADC3_INP1): Used for single-ended input.
 *     - PF12 (ADC3_INN1): Used as negative input for differential mode.
 * - ADC reference voltage: 3.3V
 */


 #pragma once

 #include "stm32h7xx_hal.h"
 #include "main.h"
 #include "peripherals/adc/utils.h"
 #include "HaGeneral/HaGeneral_config.h"
 
 #define ADC1_POLLING_OR_DMA_MODE  ADC_POLLING_MODE
 #define ADC1_POLLING_TIMEOUT      100
 #define ADC1_RESOLUTION           16
 
 
 #ifndef ADC1_HANDLE_STATUS
 #error "ADC1_HANDLE_STATUS is not defined. Please define it in HaGeneral_config.h (e.g., #define ADC1_HANDLE_STATUS HANDLE_ON)"
 #endif
 
 #ifndef ADC1_SINGLE_OR_DOUBLE_ENDED
 #error "ADC1_SINGLE_OR_DOUBLE_ENDED is not defined. Please define it in HaGeneral_config.h (e.g., #define ADC1_SINGLE_OR_DOUBLE_ENDED ADC_DIFFERENTIAL_ENDED)"
 #endif
 
 #ifndef ADC1_POLLING_OR_DMA_MODE
 #error "ADC1_POLLING_OR_DMA_MODE is not defined. Please define it in HaGeneral_config.h (e.g., #define ADC1_POLLING_OR_DMA_MODE ADC_POLLING_MODE)"
 #endif
 
 #if ADC1_POLLING_OR_DMA_MODE == ADC_DMA_MODE
 #error "ADC1_POLLING_OR_DMA_MODE only works in polling mode. Please define it in HaGeneral_config.h (e.g., #define ADC1_POLLING_OR_DMA_MODE ADC_POLLING_MODE)"
 #endif
 
 #ifndef ADC1_ANALOG_WATCHDOG
 #error "ADC1_ANALOG_WATCHDOG is not defined. Please define it in HaGeneral_config.h (e.g., #define ADC1_ANALOG_WATCHDOG HANDLE_ON)"
 #endif
 
 
 #if (ADC1_ANALOG_WATCHDOG == HANDLE_ON)
 
 #define ADC1_MAX_VALUE  ((1 << ADC1_RESOLUTION) - 1) // 2^16 -1
 #define ADC1_MIN_VALUE  0
 
 // validate high threshold
 #if (ADC1_ANALOG_WATCHDOG_HIGH_THRESHOLD < ADC1_MIN_VALUE) || (ADC1_ANALOG_WATCHDOG_HIGH_THRESHOLD > ADC1_MAX_VALUE)
 #error "ADC1_ANALOG_WATCHDOG_HIGH_THRESHOLD must be in the range 0 - 64 * 1024"
 #endif
 // Validate low threshold
 #if (ADC1_ANALOG_WATCHDOG_LOW_THRESHOLD < ADC1_MIN_VALUE) || (ADC1_ANALOG_WATCHDOG_LOW_THRESHOLD > ADC1_MAX_VALUE)
 #error "ADC1_ANALOG_WATCHDOG_LOW_THRESHOLD must be in the range 0 - 64 * 1024"
 #endif
 
 #endif
 
 
 /**
  * @brief Reads a single ADC value from ADC1 using polling mode.
  * 
  * This function wraps the generic adc_polling_get_value function, 
  * specifically for ADC1 with a predefined polling timeout.
  * 
  * @param[out] adc_value Pointer to a variable where the converted ADC value will be stored.
  * 
  * @return HAL_StatusTypeDef HAL status code indicating success or failure of the ADC read operation.
  */
 HAL_StatusTypeDef adc1_get_value(uint16_t *adc_value);
 
 
 #if ADC1_ANALOG_WATCHDOG == HANDLE_ON
 
 /**
  * @brief Handles an anomaly detected by the ADC1 analog watchdog.
  * 
  * This function is called when the analog watchdog of ADC1 detects an anomaly,
  * such as the conversion value exceeding the configured high threshold or falling
  * below the low threshold. The implementation should include handling actions
  * like logging, signaling, or taking corrective measures depending on the 
  * application requirements.
  * 
  * @note Ensure that the analog watchdog has been properly configured and enabled 
  * before relying on this function. Also, consider using this in conjunction 
  * with ADC1 interrupt handling for real-time response to watchdog events.
  * 
  */
 void adc1_wdg_process_anomaly(void);
 
 #endif
 
 
 extern ADC_HandleTypeDef hadc1;
 
 
 void MX_ADC1_Init(void);
 