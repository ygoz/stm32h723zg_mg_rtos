/**
 * @file i2c_master_test.h
 * @author 
 * @brief I2C Master testing utilities for HIL mode.
 * 
 * This header provides definitions and functions to test I2C master 
 * communication in Hardware-in-the-Loop (HIL) mode. It defines the 
 * master request message, buffer sizes, and the test function 
 * for verifying master-slave data exchange.
 * 
 * @version 0.1
 * @date 2025-08-21
 * 
 * @copyright Copyright (c) 2025
 */

#pragma once

#include "stm32h7xx_hal.h"
#include "HaGeneral/HaGeneral_config.h"

/**
 * @brief Default test message sent from master to slave.
 */
#define I2C_MASTER_REQUEST      "shalom from master! can you hear me?"

#ifdef HIL_MASTER_MODE

/**
 * @brief Buffer size for I2C master receive operations.
 */
#define I2C_BUFF_SIZE       64

/**
 * @brief 7-bit I2C slave address for testing.
 * 
 * This should match the slave configuration.
 */
#define I2C_SLAVE_ADDR      0x50

/**
 * @brief Perform an I2C master test sequence.
 * 
 * This function tests master-to-slave I2C communication by:
 *  - Sending a predefined request string (@ref I2C_MASTER_REQUEST)
 *  - Waiting for the slave to respond
 *  - Receiving the expected slave response
 *  - Comparing the response with @ref I2C_SLAVE_RESPONSE_PASS
 * 
 * The test uses non-blocking sequential transmit and receive 
 * with interrupt callbacks.
 * 
 * @param hi2c Pointer to the I2C handle structure.
 * @return HAL_StatusTypeDef 
 *         - HAL_OK: Test passed successfully  
 *         - HAL_ERROR: Response mismatch or communication failure  
 *         - HAL_TIMEOUT: Timeout waiting for transmit/receive completion
 */
HAL_StatusTypeDef hil_test_i2c(I2C_HandleTypeDef *hi2c);

#endif // HIL_MASTER_MODE
