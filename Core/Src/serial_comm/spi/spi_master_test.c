/**
 * @file    spi_master_test.c
 * @brief   SPI master test routines for HIL (Hardware-in-the-Loop) communication.
 *
 * This file provides helper functions to test SPI communication between
 * the master and slave devices in HIL mode. It supports transmitting
 * a predefined request to the slave and verifying the expected response.
 *
 * The master can operate on multiple SPI instances (e.g., SPI4, SPI5)
 * and uses blocking transmit/receive calls for simplicity.
 *
 * @note    This file is compiled only when ::HIL_MASTER_MODE is defined.
 */



#include "HaGeneral/HaGeneral_config.h"



#ifdef HIL_MASTER_MODE


#include "main.h"
#include <stdio.h>
#include "serial_comm/spi/hspi4.h"
#include "serial_comm/spi/hspi5.h"
#include "serial_comm/spi/spi_hil_test.h"




// Call this to start the SPI test
HAL_StatusTypeDef hil_test_spi(SPI_HandleTypeDef *hspi)
{
    // create spi buffers
    uint8_t rx_buffer[SPI_BUFF_SIZE] = {0};
    uint8_t tx_buffer[] = SPI_MASTER_REQUEST;

    // Start TX/RX
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hspi, tx_buffer, rx_buffer, SPI_BUFF_SIZE, SPI_TIMEOUT);
    if (status != HAL_OK) {
        printf("TransmitReceive failed: %d\r\n", status);
        return status;
    }

    // Expecting slave response
    HAL_Delay(100);
    if (strncmp((char *)rx_buffer, SPI_SLAVE_RESPONSE, strlen(SPI_SLAVE_RESPONSE)) == 0) return HAL_OK;
    else return HAL_ERROR;

    return status;
}


// Handle SPI errors, not implemented.
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI4 || hspi->Instance == SPI5) {
        printf("SPI Error: 0x%lX\r\n", hspi->ErrorCode);
    }
}

#endif