/**
 * @file i2c_master_test.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-08-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#pragma once

#include "stm32h7xx_hal.h"
#include "HaGeneral/HaGeneral_config.h"


#define I2C_MASTER_REQUEST      "shalom from master! can you hear me?"


#ifdef HIL_MASTER_MODE



#define I2C_BUFF_SIZE       64
#define I2C_SLAVE_ADDR      0x50


HAL_StatusTypeDef hil_test_i2c(I2C_HandleTypeDef *hi2c);


#endif