/**
 * @file hdac1.h
 * @author Yam Goz
 * @brief STM32H723 DAC1 interface — initialization and runtime control
 * @version 0.1
 * @date 2025-06-19
 *
 * @details
 * This module provides initialization and utility functions for controlling
 * the DAC1 peripheral on STM32H7 microcontrollers. It has 3.3V refrence voltage
 * and supports both DAC channels:
 *   - Channel 1: PA4
 *   - Channel 2: PA5
 *
 * DAC1 operates with 12-bit resolution and can generate analog voltage outputs
 * from digital input values in the range [0, 4095]. It supports right-aligned data.
 * The `dac1_set_value()` function sets the output value of a specified channel.
 */


#pragma once

#include "stm32h7xx_hal.h"
#include "main.h"
#include "HaGeneral/HaGeneral_config.h"
#include <stdint.h>
#include <stm32h7xx_hal_dac.h>
#include "mongoose.h"


extern DAC_HandleTypeDef hdac1;


void MX_DAC1_Init(void);


/**
 * @brief Set a 12-bit output value on DAC1 Channel 1 or Channel 2.
 *
 * This function sets the analog output value for one of the DAC1 channels
 * (either Channel 1 or Channel 2). It performs runtime checks to ensure that:
 * - The channel is valid (must be DAC_CHANNEL_1 or DAC_CHANNEL_2)
 * - The value is within the 12-bit range (0 to 4095)
 *
 * If the input is valid, the function ensures the DAC is started and writes
 * the specified value using right alignment (DAC_ALIGN_12B_R).
 *
 * @param dac_value 12-bit unsigned value to write to the DAC (range: 0–4095).
 * @param channel DAC channel to write to. Must be either DAC_CHANNEL_1 (PA4) or DAC_CHANNEL_2 (PA5).
 * @return HAL_StatusTypeDef 
 *         - HAL_OK if the DAC value was successfully written
 *         - HAL_ERROR if the inputs are invalid
 *         - Other HAL status codes if DAC start or value setting fails
 *
 * @note This function assumes that `hdac1` has already been initialized with `MX_DAC1_Init()`.
 * @note The DAC is started automatically if not already running.
 * @note Uses MG_INFO() logging macro for error diagnostics.
 *
 * @see MX_DAC1_Init()
 * @see HAL_DAC_Start()
 * @see HAL_DAC_SetValue()
 */
HAL_StatusTypeDef dac1_set_value(uint32_t dac_value, uint32_t channel);





