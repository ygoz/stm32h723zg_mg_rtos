/**
 * @file i2c_hil_test.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-19
 * 
 * @copyright Copyright (c) 2025
 * 
 */



#pragma once

#include "stm32h7xx_hal.h"
#include "HaGeneral/HaGeneral_config.h"
#include "serial_comm/i2c/hi2c1.h"



#ifdef HIL_TEST_MODE

HAL_StatusTypeDef I2C1_Start_Receive(void);

#endif