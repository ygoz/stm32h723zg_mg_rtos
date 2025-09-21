/**
 * @file spi_hil_test.h
 * @author 
 * @brief SPI HIL (Hardware-In-Loop) test interface.
 *
 * This header defines the interface for running SPI-based
 * hardware-in-the-loop (HIL) communication tests between
 * a master and a slave device.
 *
 * ## Test Flow
 * The SPI HIL test is designed to verify end-to-end
 * communication between master and slave:
 *
 * 1. **Master → Slave:**  
 *    The master sends a predefined request string
 *    (`SPI_MASTER_REQUEST`) over SPI.
 *
 * 2. **Slave Processing:**  
 *    The slave receives the request, validates it, and prepares
 *    the predefined response string (`SPI_SLAVE_RESPONSE`). 
 *    If the slave recieves an unknown request, it sets 'NACK' as the response.
 *
 * 3. **Slave → Master:**  
 *    The slave transmits the response back to the master.
 *
 * 4. **Master Validation:**  
 *    The master receives the response and checks if it matches
 *    the expected `SPI_SLAVE_RESPONSE`.
 *
 * 5. **Result:**  
 *    - If the response matches → test passes (`HAL_OK`).  
 *    - If not → test fails (`HAL_ERROR`).  
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


/**
 * @brief Buffer size for SPI operations.
 */
#define SPI_BUFF_SIZE       64


#ifdef HIL_MASTER_MODE


/**
 * @brief Perform a hardware-in-the-loop (HIL) SPI test as the master.
 *
 * This function sends a predefined request string (`SPI_MASTER_REQUEST`)
 * to the SPI slave device and waits for a response using a blocking
 * full-duplex transmit/receive (`HAL_SPI_TransmitReceive`).
 *
 * The function then checks if the response from the slave matches
 * the expected string (`SPI_SLAVE_RESPONSE`). If so, the test
 * passes and HAL_OK is returned. Otherwise, HAL_ERROR is returned.
 *
 * @param hspi Pointer to the SPI handle structure that defines
 *             the SPI configuration and hardware instance.
 *
 * @return HAL_StatusTypeDef
 *         - HAL_OK    : Transmission and expected response succeeded  
 *         - HAL_ERROR : Slave response mismatch  
 *         - Other     : HAL error codes from HAL_SPI_TransmitReceive  
 *
 * @note This is a blocking function and will wait up to `SPI_TIMEOUT`
 *       for the transmit/receive operation to complete.
 *
 * @see HAL_SPI_TransmitReceive
 */
HAL_StatusTypeDef hil_test_spi(SPI_HandleTypeDef *hspi);


#endif // HIL_MASTER_MODE



#ifdef HIL_SLAVE_MODE


/**
 * @brief  Initialize SPI slave interfaces and start listening for master transactions.
 *
 * This function configures both SPI4 and SPI5 peripherals to operate
 * in interrupt-driven transmit/receive (full-duplex) mode. It arms the
 * SPI hardware so that the slave is ready to respond immediately to a
 * master transaction without blocking.
 *
 * Both SPI4 and SPI5 share the same global TX and RX buffers
 * (::spi_slave_tx_buffer and ::spi_slave_rx_buffer). The size of the
 * transaction is fixed to ::SPI_BUFF_SIZE.
 *
 * @retval HAL_OK    Initialization successful and both SPI4/SPI5 are armed.
 * @retval HAL_ERROR Failed to arm either SPI4 or SPI5.
 *
 * @note This function only starts the first transaction. After each
 *       transfer completes, the application is responsible for rearming
 *       the slave inside the @ref HAL_SPI_TxRxCpltCallback callback
 *       using @ref HAL_SPI_TransmitReceive_IT again.
 *
 * @warning Using a shared RX/TX buffer for multiple SPI instances may
 *          cause data collisions if both interfaces are active at the
 *          same time. If SPI4 and SPI5 are expected to run concurrently,
 *          use separate buffers for each peripheral.
 */
HAL_StatusTypeDef spi_slave_init();


#endif


#endif