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
HAL_StatusTypeDef spi_slave_init(SPI_HandleTypeDef *hspi)
{
    // Start listening for data from the master
    // if (HAL_SPI_Receive_IT(&hspi4, spi_slave_rx_buffer, SPI_BUFF_SIZE) != HAL_OK) {
    if (HAL_SPI_TransmitReceive_IT(&hspi4, spi_slave_tx_buffer, spi_slave_rx_buffer, SPI_BUFF_SIZE) != HAL_OK) {
        return HAL_ERROR;
    }
    return HAL_OK;
}




// This function is called when a transmit/receive transaction is finished
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI4) {
        printf("mfkxnjdfbnnvxmfmxvkdfnnvx\r\n\r\n\r\n");
        // Process the received data
        // process_spi_data(spi_slave_rx_buffer, sizeof(spi_slave_rx_buffer));

        // Optionally prepare the next TX buffer here
        // prepare_spi_tx_data(spi_slave_tx_buffer, sizeof(spi_slave_tx_buffer));

        // Re-enable listening for the next transfer
        HAL_SPI_TransmitReceive_IT(hspi,
                                   spi_slave_tx_buffer,
                                   spi_slave_rx_buffer,
                                   SPI_BUFF_SIZE);
    }
}






#endif
