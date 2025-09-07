/**
 * @file spi_hil_test.h
 * @author 
 * @brief SPI HIL (Hardware-In-Loop) test interface.
 *
 *
 * ## Test Flow:
 *  1. 
 *
 * @version 0.1
 * @date 2025-08-21
 *
 * @copyright Copyright (c)
 */

#pragma once

#include "stm32h7xx_hal.h"
#include "HaGeneral/HaGeneral_config.h"


#ifdef HIL_TEST_MODE




/**
 * @brief Default test message sent from master to slave.
 */
#define SPI_MASTER_REQUEST      "shalom from master! can you hear me?"


/**
 * @brief Default response message sent by the SPI slave
 *        when a valid master request is received.
 */
#define SPI_SLAVE_RESPONSE      "shalom master! this is slave."




#ifdef HIL_MASTER_MODE

/**
 * @brief Buffer size for SPI master receive operations.
 */
#define SPI_BUFF_SIZE       64

/**
 * @brief 
 * 
 * @param hspi 
 * @return * HAL_StatusTypeDef 
 */
HAL_StatusTypeDef hil_test_spi(SPI_HandleTypeDef *hspi);


#endif // HIL_MASTER_MODE



#ifdef HIL_SLAVE_MODE

/**
 * @brief Size of the SPI receive buffer.
 */
#define SPI_BUFF_SIZE           64



HAL_StatusTypeDef spi_slave_init(void);

#endif


#endif