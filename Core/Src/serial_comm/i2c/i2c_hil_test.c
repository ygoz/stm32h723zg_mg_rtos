#include "main.h"
#include <stdio.h>
#include "HaGeneral/HaGeneral_config.h"
#include "serial_comm/i2c/hi2c1.h"

#ifdef HIL_TEST_MODE

static uint8_t rx_buffer[3];          // Receive buffer (matches master send size)
static uint8_t tx_buffer[] = "hi";    // Response buffer

#define RxSIZE  32
uint8_t RxData[RxSIZE];

int count = 0;

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
    if (TransferDirection == I2C_DIRECTION_TRANSMIT)  // Master writing to slave
    {
        printf("Master wants to transmit\r\n");

        // Start receiving the first byte
        count = 0;  // reset count for new transfer
        HAL_I2C_Slave_Sequential_Receive_IT(hi2c, &RxData[count], 1, I2C_FIRST_FRAME);
    }
    else
    {
        printf("Master requests data (read) - not handled\r\n");
    }
}

// Called each time a byte is received
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    count++;  // increment received byte count
    printf("Received byte %d: %u\r\n", count, RxData[count-1]);

    if (count < RxSIZE)
    {
        // Prepare to receive next byte
        HAL_I2C_Slave_Sequential_Receive_IT(hi2c, &RxData[count], 1, I2C_NEXT_FRAME);
    }
    else
    {
        printf("Buffer full, stopping reception\r\n");
    }
}


void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(hi2c);
}


#endif