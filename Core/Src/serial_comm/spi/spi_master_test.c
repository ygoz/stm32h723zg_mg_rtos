#include "main.h"
#include <stdio.h>
#include "HaGeneral/HaGeneral_config.h"
#include "serial_comm/spi/hspi4.h"
#include "serial_comm/spi/hspi5.h"
#include "serial_comm/spi/spi_hil_test.h"


#ifdef HIL_MASTER_MODE



static uint8_t tx_buffer[] = SPI_MASTER_REQUEST;
static uint8_t rx_buffer[SPI_BUFF_SIZE] = {0};


HAL_StatusTypeDef hil_test_spi(SPI_HandleTypeDef *hspi)
{
    // TODO --> clear rx buff
    // Send test string to slave
    HAL_StatusTypeDef status = HAL_SPI_Transmit(hspi, tx_buffer, sizeof(tx_buffer), HAL_MAX_DELAY);
    if (status != HAL_OK)
        return status;

    // HAL_Delay(1); // or wait for an IRQ/flag

    // Receive response from slave
    status = HAL_SPI_Receive(hspi, rx_buffer, sizeof(SPI_SLAVE_RESPONSE), HAL_MAX_DELAY);
    if (status != HAL_OK)
        return status;

    // Compare response with expected
    if (strncmp((char *)rx_buffer, SPI_SLAVE_RESPONSE, strlen(SPI_SLAVE_RESPONSE)) == 0)
        return HAL_OK;
    else
        return HAL_ERROR;
}




#endif