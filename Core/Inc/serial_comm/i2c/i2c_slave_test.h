/**
 * @file i2c_slave_test.h
 * @author 
 * @brief I2C Slave test interface for HIL (Hardware-In-Loop) testing.
 *
 * This module provides initialization and callback handling
 * for I2C slave devices used in HIL test mode. It defines
 * buffers, response messages, and initialization APIs for
 * handling master-slave I2C communication.
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
 * @brief Default response message sent by the I2C slave
 *        when a valid master request is received.
 */
#define I2C_SLAVE_RESPONSE_PASS "shalom master! this is slave."

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
