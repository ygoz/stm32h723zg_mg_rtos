/**
 * @file i2c_hil_test.h
 * @author 
 * @brief I2C HIL (Hardware-In-Loop) test interface.
 *
 * This module provides initialization functions and callback handlers
 * for I2C master–slave communication during HIL testing. It defines
 * the data buffers, expected request/response messages, and APIs used
 * to verify correct communication.
 *
 * ## Test Flow:
 *  1. The master sends an I2C request packet to the slave.
 *  2. The slave checks if the received request matches the expected data.
 *  3. If valid, the slave sends back the predefined response.
 *  4. The master receives the response and compares it against its expected data.
 *  5. If the response matches, the test returns HAL_OK; otherwise, HAL_ERROR.
 *
 * @version 0.1
 * @date 2025-08-21
 *
 * @copyright Copyright (c)
 */

#pragma once

#include "stm32h7xx_hal.h"
#include "HaGeneral/HaGeneral_config.h"



/**
 * @brief Default test message sent from master to slave.
 */
#define I2C_MASTER_REQUEST      "shalom from master! can you hear me?"


/**
 * @brief Default response message sent by the I2C slave
 *        when a valid master request is received.
 */
#define I2C_SLAVE_RESPONSE_PASS "shalom master! this is slave."




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



#ifdef HIL_SLAVE_MODE

/**
 * @brief Size of the I2C receive buffer.
 */
#define I2C_BUFF_SIZE           64

/**
 * @brief Initialize the I2C slave interface.
 *
 * This function enables the I2C peripheral in listening mode
 * on both `hi2c1` and `hi2c4`. The slave will wait for master
 * requests and respond accordingly.
 *
 * @return HAL status:
 *         - HAL_OK on success.
 *         - HAL_ERROR if initialization fails.
 */
HAL_StatusTypeDef i2c_slave_init(void);

#endif
