/**
 * @file hadc3.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */
// 12 bit resolution
//  PC3_C     ------> ADC3_INP1 --> if single ended use this pin
//  PC2_C     ------> ADC3_INN1 --> used for differential mode
// 3.3 voltage?

#pragma once

#include "stm32h7xx_hal.h"
#include "main.h"
#include "peripherals/adc/utils.h"
#include "HaGeneral/HaGeneral_config.h"


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

extern uint16_t adc3_dma_buffer[ADC3_DMA_BUFFER_SIZE];

uint16_t * adc3_get_value(void);

#elif ADC3_POLLING_OR_DMA_MODE == ADC_POLLING_MODE

uint16_t adc3_get_value(void);

#endif


//dma 
extern DMA_HandleTypeDef hdma_adc3;


extern ADC_HandleTypeDef hadc3;

void MX_ADC3_Init(void);

void MX_BDMA_Init(void);