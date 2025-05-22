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


#pragma once

#include "stm32h7xx_hal.h"
#include "main.h"
#include "serial_comm/i2c/utils.h"
#include "HaGeneral/HaGeneral_config.h"


#ifndef ADC3_HANDLE_STATUS
#error "ADC3_HANDLE_STATUS is not defined. Please define it in HaGeneral_config.h (e.g., #define ADC3_HANDLE_STATUS HANDLE_ON)"
#endif

extern ADC_HandleTypeDef hadc3;

void MX_ADC3_Init(void);

uint16_t adc3_get_value(void);