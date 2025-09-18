#include "main.h"
#include <stdio.h>
#include "HaGeneral/HaGeneral_config.h"
#include "serial_comm/spi/hspi4.h"
#include "serial_comm/spi/hspi5.h"
#include "serial_comm/spi/spi_hil_test.h"

#ifdef HIL_SLAVE_MODE

#include "string.h"

static uint8_t spi_slave_rx_buffer[SPI_BUFF_SIZE];

// Call this once in your init code
HAL_StatusTypeDef spi_slave_init(SPI_HandleTypeDef *hspi)
{
    // Start listening for data from the master
    if (HAL_SPI_Receive_IT(&hspi4, spi_slave_rx_buffer, SPI_BUFF_SIZE) != HAL_OK) {
        return HAL_ERROR;
    }
    return HAL_OK;
}




void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI4)   // Make sure it's the correct SPI instance
    {
        // Ensure null-termination
        spi_slave_rx_buffer[SPI_BUFF_SIZE-1] = '\0';

        printf("\r\nSPI Slave received: %s\r\n", spi_slave_rx_buffer);

        if (strncmp((char*)spi_slave_rx_buffer, SPI_MASTER_REQUEST, strlen(SPI_MASTER_REQUEST)) == 0) {
            printf("Message matches expected request!\r\n");
            HAL_StatusTypeDef status = HAL_SPI_Transmit_IT(hspi, (uint8_t*)SPI_SLAVE_RESPONSE, strlen(SPI_SLAVE_RESPONSE));
            printf("TX status: %d\r\n", status);
        }

        // Clear RX buffer for next transfer
        memset(spi_slave_rx_buffer, 0, SPI_BUFF_SIZE);

        if (hspi->State == HAL_SPI_STATE_READY) {
            // Re-arm RX so we keep listening
            HAL_SPI_Receive_IT(hspi, spi_slave_rx_buffer, SPI_BUFF_SIZE);
        }
            printf("\r\n\r\ndone?\r\n\r\n");

    }
}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI4)
    {
        printf("SPI DATA WAS SENT...\r\n");


    }
}




#endif
