#include "main.h"
#include <stdio.h>
#include "HaGeneral/HaGeneral_config.h"
#include "serial_comm/spi/hspi4.h"
#include "serial_comm/spi/hspi5.h"
#include "serial_comm/spi/spi_hil_test.h"


#ifdef HIL_SLAVE_MODE


#include "string.h"
#include "stdio.h"



uint8_t rx_buffer[SPI_BUFF_SIZE];

// Call this once in your init code
HAL_StatusTypeDef spi_slave_init(void)
{
    // Start listening for data from the master
    if (HAL_SPI_Receive_IT(&hspi4, rx_buffer, SPI_BUFF_SIZE) != HAL_OK) {
        return HAL_ERROR;
    }
    // if (HAL_SPI_Receive_IT(&hspi5, rx_buffer, SPI_BUFF_SIZE) != HAL_OK) {
    //     return HAL_ERROR;
    // }
        return HAL_OK;
}

// This callback is called when data is received
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
    // printf("poopo");
    if (hspi->Instance == SPI4)   // Make sure it's the correct SPI instance
    {
        // Print the received data (assuming it's a string)
        printf("SPI Slave received: %s\r\n", rx_buffer);

        // Re-arm the RX interrupt so we keep listening
        HAL_SPI_Receive_IT(&hspi4, rx_buffer, SPI_BUFF_SIZE);
    }
}



#endif