#include "main.h"
#include <stdio.h>
#include "HaGeneral/HaGeneral_config.h"
#include "serial_comm/spi/hspi4.h"
#include "serial_comm/spi/hspi5.h"
#include "serial_comm/spi/spi_hil_test.h"


#ifdef HIL_SLAVE_MODE


#include "string.h"
#include "stdio.h"



static uint8_t spi_slave_rx_buffer[SPI_BUFF_SIZE];

// Call this once in your init code
HAL_StatusTypeDef spi_slave_init(void)
{
    // Start listening for data from the master
    if (HAL_SPI_Receive_IT(&hspi4, spi_slave_rx_buffer, SPI_BUFF_SIZE) != HAL_OK) {
        return HAL_ERROR;
    }
    // if (HAL_SPI_Receive_IT(&hspi5, spi_slave_rx_buffer, SPI_BUFF_SIZE) != HAL_OK) {
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
        printf("\r\nSPI Slave received: %s\r\n", spi_slave_rx_buffer);
        if (strncmp((char*)spi_slave_rx_buffer, SPI_MASTER_REQUEST, strlen(SPI_MASTER_REQUEST)) == 0){
            printf("Message matches expected request!\r\n\r\n\r\n");
        }

        // Re-arm the RX interrupt so we keep listening
        HAL_SPI_Receive_IT(hspi, spi_slave_rx_buffer, SPI_BUFF_SIZE);
    }
}



#endif



/*
#include "main.h"
#include <stdio.h>
#include "HaGeneral/HaGeneral_config.h"
#include "serial_comm/spi/hspi4.h"
#include "serial_comm/spi/hspi5.h"
#include "serial_comm/spi/spi_hil_test.h"


#ifdef HIL_SLAVE_MODE


#include "string.h"
#include "stdio.h"



static uint8_t spi_slave_rx_buffer[SPI_BUFF_SIZE];

// Call this once in your init code
HAL_StatusTypeDef spi_slave_init(void)
{
    // Start listening for data from the master
    if (HAL_SPI_Receive_IT(&hspi4, spi_slave_rx_buffer, SPI_BUFF_SIZE) != HAL_OK) {
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
    if (hspi->Instance == SPI4 || hspi->Instance == SPI5)   // Make sure it's the correct SPI instance
    {
        // Print the received data (assuming it's a string)
        printf("SPI Slave received: %s\r\n", spi_slave_rx_buffer);

        if (strncmp((char*)spi_slave_rx_buffer, SPI_MASTER_REQUEST, strlen(SPI_MASTER_REQUEST)) == 0) {
            printf("Message matches expected request!\r\n");
            // if (HAL_OK == HAL_SPI_Transmit_IT(hspi, SPI_SLAVE_RESPONSE, strlen(SPI_SLAVE_RESPONSE))){
            //     printf("expecting good news...\r\n");
            // }
        } else {
            // HAL_SPI_Receive_IT(hspi, spi_slave_rx_buffer, SPI_BUFF_SIZE);
            printf("Message does not match. Expected: \"%s\"\r\n", SPI_MASTER_REQUEST);
        }

        memset(spi_slave_rx_buffer, 0, SPI_BUFF_SIZE); // clean rx buffer for new transfer
        // Re-arm the RX interrupt so we keep listening
        HAL_SPI_Receive_IT(hspi, spi_slave_rx_buffer, SPI_BUFF_SIZE);
    }
}


// void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
// {
//     if (hspi->Instance == SPI4 || hspi->Instance == SPI5)
//     {
//         // Re-arm RX after transmit finishes
//         HAL_SPI_Receive_IT(hspi, spi_slave_rx_buffer, SPI_BUFF_SIZE);
//     }
// }




#endif


*/