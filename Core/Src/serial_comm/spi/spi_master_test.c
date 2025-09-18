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
    memset(rx_buffer, 0, sizeof(rx_buffer));

    HAL_StatusTypeDef status;
    uint32_t tries = 0;
    const uint32_t max_tries = 3;   // 1st for request, 2nd for response

    do {
        status = HAL_SPI_TransmitReceive(hspi,
                                         tx_buffer,
                                         rx_buffer,
                                         sizeof(tx_buffer),
                                         HAL_MAX_DELAY);

        if (status != HAL_OK) {
            printf("TransmitReceive failed: %d\r\n", status);
            return status;
        }

        printf("Master received (try %lu): %s\r\n", tries, rx_buffer);

        tries++;

        HAL_Delay(40);

    } while ((rx_buffer[0] == '\0') && (tries < max_tries));

    if (strncmp((char *)rx_buffer, SPI_SLAVE_RESPONSE, strlen(SPI_SLAVE_RESPONSE)) == 0) {
        printf("Master match OK\r\n");
        return HAL_OK;
    } else {
        printf("Master mismatch after %lu tries, expected: %s, got: %s\r\n",
               tries, SPI_SLAVE_RESPONSE, (char *)rx_buffer);
        return HAL_ERROR;
    }
}




#endif