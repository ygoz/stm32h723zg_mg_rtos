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
// voltage?

#pragma once

#include "stm32h7xx_hal.h"
#include "main.h"
#include "serial_comm/i2c/utils.h"
#include "HaGeneral/HaGeneral_config.h"


#ifndef ADC3_HANDLE_STATUS
#error "ADC3_HANDLE_STATUS is not defined. Please define it in HaGeneral_config.h (e.g., #define ADC3_HANDLE_STATUS HANDLE_ON)"
#endif

#ifndef ADC3_SINGLE_OR_DOUBLE_ENDED
#error "ADC3_SINGLE_OR_DOUBLE_ENDED is not defined. Please define it in HaGeneral_config.h (e.g., #define ADC3_SINGLE_OR_DOUBLE_ENDED ADC_DIFFERENTIAL_ENDED)"
#endif



extern ADC_HandleTypeDef hadc3;

void MX_ADC3_Init(void);

uint16_t adc3_get_value(void);