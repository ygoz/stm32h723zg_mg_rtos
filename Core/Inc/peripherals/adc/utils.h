/**
 * @file utils.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "peripherals/adc/hadc3.h"

#define HANDLE_ON  1
#define HANDLE_OFF 0


typedef enum {
    ADC_STATUS_NONE      = 0,  ///< No ADCs initialized successfully
    ADC_STATUS_ADC1      = 1,  ///< Only ADC1 initialized
    ADC_STATUS_ADC2      = 2,  ///< Only ADC2 initialized
    ADC_STATUS_ADC3      = 4,  ///< Only ADC3 initialized
    ADC_STATUS_ADC1_ADC2 = 3,  ///< ADC1 and ADC2 initialized
    ADC_STATUS_ADC1_ADC3 = 5,  ///< ADC1 and ADC3 initialized
    ADC_STATUS_ADC2_ADC3 = 6,  ///< ADC2 and ADC3 initialized
    ADC_STATUS_ALL       = 7   ///< All ADCs initialized successfully
} ADC_StatusBitmap;


uint8_t adc_init_all_handles(void);
uint16_t adc_get_value(ADC_HandleTypeDef *hadc);