#include "main.h"
#include <stdio.h>
#include "HaGeneral/HaGeneral_config.h"
#include "serial_comm/i2c/hi2c1.h"

#ifdef HIL_TEST_MODE

static uint8_t rx_buffer[3];          // Receive buffer (matches master send size)
static uint8_t tx_buffer[] = "hi";    // Response buffer

#define RxSIZE  6
uint8_t RxData[RxSIZE];

int count = 0;

void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(hi2c);
}

void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
	if (TransferDirection == I2C_DIRECTION_TRANSMIT)  // if the master wants to transmit the data
	{
        printf("tx\r\n");
		// receive using sequential function.
		// The I2C_FIRST_AND_LAST_FRAME implies that the slave will send a NACK after receiving "entered" num of bytes
		HAL_I2C_Slave_Sequential_Receive_IT(hi2c, RxData, 3, I2C_FIRST_AND_LAST_FRAME);
	}
	else  // if the master requests the data from the slave
	{
        printf("rx\r\n");

		// Error_Handler();  // call error handler
	}
}

void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	// count++;
    printf("Received bytes: ");
    for (int i = 0; i < RxSIZE; i++) {
        printf("%u ", RxData[i]);  // Print each byte as a number
    }
    printf("\r\n");

}

void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(hi2c);
}


#endif