/**
 * @file    spi_slave_test.c
 * @brief   SPI slave test routines for HIL (Hardware-in-the-Loop) communication.
 *
 * This file provides helper functions for SPI slave operation in HIL mode.
 * It sets up SPI4 and SPI5 as interrupt-driven full-duplex slaves, listens
 * for requests from the master, and responds with either a predefined response
 * (::SPI_SLAVE_RESPONSE) or "NACK" if the request is invalid.
 *
 * @version 0.1
 * @date    2025-09-21
 * @copyright Copyright (c) 2025
 */

#include "main.h"
#include <stdio.h>
#include "HaGeneral/HaGeneral_config.h"
#include "serial_comm/spi/hspi4.h"
#include "serial_comm/spi/hspi5.h"
#include "serial_comm/spi/spi_hil_test.h"


#ifdef HIL_SLAVE_MODE


#include "string.h"


static uint8_t spi_slave_rx_buffer[SPI_BUFF_SIZE];
static uint8_t spi_slave_tx_buffer[] = SPI_SLAVE_RESPONSE;


// Call this once in your init code
HAL_StatusTypeDef spi_slave_init()
{
    // Start listening for data from the master
    if (HAL_SPI_TransmitReceive_IT(&hspi4, spi_slave_tx_buffer, spi_slave_rx_buffer, SPI_BUFF_SIZE) != HAL_OK) {
        return HAL_ERROR;
    }
    if (HAL_SPI_TransmitReceive_IT(&hspi5, spi_slave_tx_buffer, spi_slave_rx_buffer, SPI_BUFF_SIZE) != HAL_OK) {
        return HAL_ERROR;
    }
    return HAL_OK;
}



/**
 * @brief  Process received SPI data and prepare a response.
 *
 * This helper function checks the content of the SPI RX buffer
 * against the expected master request string. If the request
 * matches ::SPI_MASTER_REQUEST, it copies the predefined
 * ::SPI_SLAVE_RESPONSE into the TX buffer. Otherwise, it loads
 * "NACK" as the response.
 *
 * @param[in]  rx_buf Pointer to the buffer holding data received from the master.
 * @param[out] tx_buf Pointer to the buffer to be populated with the response.
 */
static void process_spi_data(uint8_t *rx_buf, uint8_t *tx_buf)
{
    // Clear TX buffer
    memset(tx_buf, 0, strlen(SPI_SLAVE_RESPONSE));

    if (strncmp((char *)rx_buf, SPI_MASTER_REQUEST, strlen(SPI_MASTER_REQUEST)) == 0)
    {
        // Copy the predefined response
        strncpy((char *)tx_buf, SPI_SLAVE_RESPONSE, strlen(SPI_SLAVE_RESPONSE));
    }
    else
    {
        // Send NACK
        strncpy((char *)tx_buf, "NACK", strlen(SPI_SLAVE_RESPONSE));
    }
}




// This function is called when a transmit/receive transaction is finished
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI4 || hspi->Instance == SPI5) {
        
        printf("\r\n\r\nSPI message recieved, processing...\r\n");

        // Process the received data
        process_spi_data(spi_slave_rx_buffer, spi_slave_tx_buffer);

        // Re-enable listening for the next transfer
        HAL_SPI_TransmitReceive_IT(hspi,
                                   spi_slave_tx_buffer,
                                   spi_slave_rx_buffer,
                                   SPI_BUFF_SIZE);
    }
}






#endif
