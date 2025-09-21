#include "HaGeneral/HaGeneral_config.h"



#ifdef HIL_MASTER_MODE


#include "main.h"
#include <stdio.h>
#include "serial_comm/spi/hspi4.h"
#include "serial_comm/spi/hspi5.h"
#include "serial_comm/spi/spi_hil_test.h"

static uint8_t tx_buffer[] = SPI_MASTER_REQUEST;
static uint8_t rx_buffer[SPI_BUFF_SIZE] = {0};
static volatile uint32_t tries = 0;
static volatile uint32_t max_tries = 2;
static volatile HAL_StatusTypeDef spi_status = HAL_ERROR;
static volatile uint8_t spi_busy = 0;

// Call this to start the SPI test
HAL_StatusTypeDef hil_test_spi(SPI_HandleTypeDef *hspi)
{
    // Clear buffer and reset state
    memset(rx_buffer, 0, sizeof(rx_buffer));
    tries = 0;
    spi_status = HAL_ERROR;
    spi_busy = 1;  // Indicate SPI operation in progress

    // Start first TX/RX
    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hspi, tx_buffer, rx_buffer, SPI_BUFF_SIZE, SPI_TIMEOUT);
    if (status != HAL_OK) {
        printf("TransmitReceive failed: %d\r\n", status);
        // spi_busy = 0;
        return status;
    }
    if (strncmp((char *)rx_buffer, SPI_SLAVE_RESPONSE, strlen(SPI_SLAVE_RESPONSE)) == 0) return HAL_OK;
    else return HAL_ERROR;

    return status;
}

// SPI TX/RX completion callback
void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{

    printf("callbacj triggered");


    if (hspi->Instance == SPI4)  // Adjust for your SPI instance
    {
        printf("Master received (try %lu): %s\r\n", tries, rx_buffer);

        // Check if retry needed
        if (rx_buffer[0] == '\0' && tries < max_tries - 1) {
            tries++;

            // Start next TX/RX
            HAL_StatusTypeDef status = HAL_SPI_TransmitReceive_IT(hspi, tx_buffer, rx_buffer, SPI_BUFF_SIZE);
            if (status != HAL_OK) {
                printf("TransmitReceive_IT failed: %d\r\n", status);
                spi_status = status;
                spi_busy = 0;
            }
        } else {
            // Check result
            if (strncmp((char *)rx_buffer, SPI_SLAVE_RESPONSE, strlen(SPI_SLAVE_RESPONSE)) == 0) {
                printf("Master match OK\r\n");
                spi_status = HAL_OK;
            } else {
                printf("\r\nMaster mismatch after %d tries, expected: %s, got: %s\r\n",
                       tries, SPI_SLAVE_RESPONSE, (char *)rx_buffer);
                spi_status = HAL_ERROR;
            }
            spi_busy = 0;  // Test complete
        }
    }
}

// Optional: Handle SPI errors
void HAL_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
    if (hspi->Instance == SPI4) {
        printf("SPI Error: 0x%lX\r\n", hspi->ErrorCode);
        spi_status = HAL_ERROR;
        spi_busy = 0;
    }
}

#endif