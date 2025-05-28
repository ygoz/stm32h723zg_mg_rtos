/**
 * @file hadc2.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#pragma once

#include "stm32h7xx_hal.h"
#include "main.h"
#include "peripherals/adc/utils.h"
#include "HaGeneral/HaGeneral_config.h"


#define ADC2_POLLING_TIMEOUT 100
#define ADC2_RESOLUTION      16


#ifndef ADC2_HANDLE_STATUS
#error "ADC2_HANDLE_STATUS is not defined. Please define it in HaGeneral_config.h (e.g., #define ADC2_HANDLE_STATUS HANDLE_ON)"
#endif

#ifndef ADC2_SINGLE_OR_DOUBLE_ENDED
#error "ADC2_SINGLE_OR_DOUBLE_ENDED is not defined. Please define it in HaGeneral_config.h (e.g., #define ADC2_SINGLE_OR_DOUBLE_ENDED ADC_DIFFERENTIAL_ENDED)"
#endif

#ifndef ADC2_POLLING_OR_DMA_MODE
#error "ADC2_POLLING_OR_DMA_MODE is not defined. Please define it in HaGeneral_config.h (e.g., #define ADC2_POLLING_OR_DMA_MODE ADC_POLLING_MODE)"
#endif

#ifndef ADC2_ANALOG_WATCHDOG
#error "ADC2_ANALOG_WATCHDOG is not defined. Please define it in HaGeneral_config.h (e.g., #define ADC2_ANALOG_WATCHDOG HANDLE_ON)"
#endif


#if (ADC2_ANALOG_WATCHDOG == HANDLE_ON)

#define ADC2_MAX_VALUE  ((1 << ADC2_RESOLUTION) - 1) // 2^12 -1
#define ADC2_MIN_VALUE  0

// validate high threshold
#if (ADC2_ANALOG_WATCHDOG_HIGH_THRESHOLD < ADC2_MIN_VALUE) || (ADC2_ANALOG_WATCHDOG_HIGH_THRESHOLD > ADC2_MAX_VALUE)
#error "ADC3_ANALOG_WATCHDOG_HIGH_THRESHOLD must be in the range 0 - 64 * 1024"
#endif
// Validate low threshold
#if (ADC2_ANALOG_WATCHDOG_LOW_THRESHOLD < ADC2_MIN_VALUE) || (ADC2_ANALOG_WATCHDOG_LOW_THRESHOLD > ADC2_MAX_VALUE)
#error "ADC2_ANALOG_WATCHDOG_LOW_THRESHOLD must be in the range 0 - 64 * 1024"
#endif

#endif


#if (ADC2_POLLING_OR_DMA_MODE == ADC_DMA_MODE)

#ifndef ADC2_DMA_BUFFER_SIZE
#error "ADC2_DMA_BUFFER_SIZE is not defined. Please define it in HaGeneral_config.h (e.g., #define ADC2_DMA_BUFFER_SIZE 256)"
#endif

#define UINT16_SIZE_BYTES 2

#if (ADC2_DMA_BUFFER_SIZE * UINT16_SIZE_BYTES > 16 * 1024) //TODO: CHANGE TO SRAM OF NEW DMA
#error "ADC2_DMA_BUFFER_SIZE exceeds the maximum buffer size of 16KB (SRAM4 size). To avoid hardfaults please reduce the buffer size."
#endif

#endif


// DMA SETTINGS
#if ADC2_POLLING_OR_DMA_MODE == ADC_DMA_MODE


extern uint16_t adc2_dma_buffer[ADC2_DMA_BUFFER_SIZE];


uint16_t * adc2_get_value(void);

#elif ADC2_POLLING_OR_DMA_MODE == ADC_POLLING_MODE


/**
 * @brief Reads a single ADC value from ADC2 using polling mode.
 * 
 * This function wraps the generic adc_polling_get_value function, 
 * specifically for ADC2 with a predefined polling timeout.
 * 
 * @param[out] adc_value Pointer to a variable where the converted ADC value will be stored.
 * 
 * @return HAL_StatusTypeDef HAL status code indicating success or failure of the ADC read operation.
 */
HAL_StatusTypeDef adc2_get_value(uint16_t *adc_value);

#endif

#if ADC2_ANALOG_WATCHDOG == HANDLE_ON

/**
 * @brief Handles an anomaly detected by the ADC2 analog watchdog.
 * 
 * This function is called when the analog watchdog of ADC2 detects an anomaly,
 * such as the conversion value exceeding the configured high threshold or falling
 * below the low threshold. The implementation should include handling actions
 * like logging, signaling, or taking corrective measures depending on the 
 * application requirements.
 * 
 * @note Ensure that the analog watchdog has been properly configured and enabled 
 * before relying on this function. Also, consider using this in conjunction 
 * with ADC2 interrupt handling for real-time response to watchdog events.
 * 
 */
void adc2_wdg_process_anomaly(void);

#endif


extern ADC_HandleTypeDef hadc2;


void MX_ADC2_Init(void);
