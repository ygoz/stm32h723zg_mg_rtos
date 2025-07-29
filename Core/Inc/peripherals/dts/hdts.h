/**
 * @file hdts.h
 * @author Yam Goz
 * @brief Digital Temperature Sensor (DTS) interface for STM32H7 series
 * 
 * @version 0.1
 * @date 2025-06-22
 * 
 * @details
 * This header declares functions and definitions to initialize,
 * control, and read from the internal Digital Temperature Sensor (DTS)
 * peripheral on STM32H7 microcontrollers.
 * 
 * The DTS peripheral provides temperature measurement capabilities
 * using internal sensor hardware and HAL support functions such as:
 * - HAL_DTS_Start()
 * - HAL_DTS_Stop()
 * - HAL_DTS_GetTemperature()
 * 
 * Typical usage involves starting the DTS, reading the temperature, 
 * stopping the DTS, and optionally setting temperature thresholds -- NOT IMPLEMENTED.
 * 
 * @note This module depends on STM32H7 HAL libraries.
 */

#pragma once

#include "stm32h7xx_hal.h"
#include <stdint.h>
#include <stm32h7xx_hal_dts.h>
#include "mongoose.h"



extern DTS_HandleTypeDef hdts;


void MX_DTS_Init(void);


/**
  * @brief Retrieves the MCU's temperature measurement from the Digital Temperature Sensor (DTS).
  * @param  Temperature  Temperature in deg C
  * @note This function fetches the most recent temperature measurement available from the DTS.
  * @retval HAL status
  */
HAL_StatusTypeDef dts_get_temperature(int32_t *temperature);