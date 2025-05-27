/**
 * @file hadc3.h
 * @author Yam Goz (you@domain.com)
 * @brief Header file for ADC3 handle configuration and pin mapping.
 * @version 0.1
 * @date 2025-05-21
 * 
 * @copyright Copyright (c) 2025
 * 
 * @details
 * This file provides the configuration for ADC3, including its resolution and pin assignments.
 * - 12-bit resolution
 * - Pin mappings:
 *     - PC3_C (ADC3_INP1): Used for single-ended input.
 *     - PC2_C (ADC3_INN1): Used as negative input for differential mode.
 * - ADC reference voltage: 3.3V
 */


#pragma once

#include "stm32h7xx_hal.h"
#include "main.h"
#include "peripherals/adc/utils.h"
#include "HaGeneral/HaGeneral_config.h"


#define ADC3_POLLING_TIMEOUT 100


#ifndef ADC3_HANDLE_STATUS
#error "ADC3_HANDLE_STATUS is not defined. Please define it in HaGeneral_config.h (e.g., #define ADC3_HANDLE_STATUS HANDLE_ON)"
#endif

#ifndef ADC3_SINGLE_OR_DOUBLE_ENDED
#error "ADC3_SINGLE_OR_DOUBLE_ENDED is not defined. Please define it in HaGeneral_config.h (e.g., #define ADC3_SINGLE_OR_DOUBLE_ENDED ADC_DIFFERENTIAL_ENDED)"
#endif

#ifndef ADC3_POLLING_OR_DMA_MODE
#error "ADC3_POLLING_OR_DMA_MODE is not defined. Please define it in HaGeneral_config.h (e.g., #define ADC3_POLLING_OR_DMA_MODE ADC_POLLING_MODE)"
#endif


#if (ADC3_POLLING_OR_DMA_MODE == ADC_DMA_MODE)

#ifndef ADC3_DMA_BUFFER_SIZE
#error "ADC3_DMA_BUFFER_SIZE is not defined. Please define it in HaGeneral_config.h (e.g., #define ADC3_DMA_BUFFER_SIZE 256)"
#endif

#define UINT16_SIZE_BYTES 2
#if (ADC3_DMA_BUFFER_SIZE * UINT16_SIZE_BYTES > 16 * 1024)
#error "ADC3_DMA_BUFFER_SIZE exceeds the maximum buffer size of 16KB (SRAM4 size). To avoid hardfaults please reduce the buffer size."
#endif

#endif


// DMA SETTINGS
#if ADC3_POLLING_OR_DMA_MODE == ADC_DMA_MODE


/**
 * @brief DMA buffer for ADC3 data.
 * 
 * This buffer holds the converted ADC3 data and is used during DMA (Direct Memory Access) operations.
 * It is specifically placed in SRAM4 because BDMA (Basic DMA) is only capable of accessing SRAM4.
 * 
 * @note 
 * The linker script is configured to place this buffer in SRAM4, which has a size limit of 16KB.
 */
extern uint16_t adc3_dma_buffer[ADC3_DMA_BUFFER_SIZE];


/**
 * @brief Gets the address of the ADC3 DMA buffer.
 * 
 * This function returns a pointer to the buffer used for storing the converted data from ADC3.
 * The buffer is used during DMA operations and is placed in SRAM4 to comply with BDMA access limitations.
 * 
 * @return uint16_t* Pointer to the start of the ADC3 DMA buffer.
 */
uint16_t * adc3_get_value(void);

#elif ADC3_POLLING_OR_DMA_MODE == ADC_POLLING_MODE


/**
 * @brief Reads a single ADC value from ADC3 using polling mode.
 * 
 * This function wraps the generic adc_polling_get_value function, 
 * specifically for ADC3 with a predefined polling timeout.
 * 
 * @param[out] adc_value Pointer to a variable where the converted ADC value will be stored.
 * 
 * @return HAL_StatusTypeDef HAL status code indicating success or failure of the ADC read operation.
 */
HAL_StatusTypeDef adc3_get_value(uint16_t *adc_value);

#endif


//dma 
extern DMA_HandleTypeDef hdma_adc3;

extern ADC_HandleTypeDef hadc3;


void MX_ADC3_Init(void);

void MX_BDMA_Init(void);