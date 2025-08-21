#include "main.h"
#include <stdio.h>
#include "HaGeneral/HaGeneral_config.h"
#include "serial_comm/i2c/hi2c1.h"
#include "serial_comm/i2c/hi2c4.h"
#include "serial_comm/i2c/i2c_master_test.h"
#include "serial_comm/i2c/i2c_slave_test.h"






#ifdef HIL_SLAVE_MODE


uint8_t i2c_slave_tx_data[] = I2C_SLAVE_RESPONSE_PASS;  // the data to send


uint8_t i2c_slave_rx_buffer[I2C_BUFF_SIZE];

int slave_rx_i2c_byte_count = 0;


void HAL_I2C_ListenCpltCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(hi2c);
}


void HAL_I2C_AddrCallback(I2C_HandleTypeDef *hi2c, uint8_t TransferDirection, uint16_t AddrMatchCode)
{
    if (hi2c->Instance == I2C1 || hi2c->Instance == I2C4){
        if (TransferDirection == I2C_DIRECTION_TRANSMIT)  // Master writing to slave
        {
            printf("Master wants to transmit\r\n");
            slave_rx_i2c_byte_count = 0;  // reset slave_rx_i2c_byte_count for new transfer
            memset(i2c_slave_rx_buffer, 0, I2C_BUFF_SIZE); // clean rx buffer for new transfer
            HAL_I2C_Slave_Sequential_Receive_IT(hi2c, &i2c_slave_rx_buffer[slave_rx_i2c_byte_count], 1, I2C_FIRST_FRAME);
        }
        else if (TransferDirection == I2C_DIRECTION_RECEIVE) // Master reading from slave
        {
            if (strncmp((char*)i2c_slave_rx_buffer, I2C_MASTER_REQUEST, strlen(I2C_MASTER_REQUEST)) == 0) {
                printf("Message matches expected request!\r\n");
                HAL_I2C_Slave_Sequential_Transmit_IT(hi2c, i2c_slave_tx_data, sizeof(i2c_slave_tx_data) - 1, I2C_FIRST_AND_LAST_FRAME);
            } else {
                printf("Message does not match. Expected: \"%s\"\r\n", I2C_MASTER_REQUEST);
            }
        }
    }
}



void HAL_I2C_SlaveTxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1 || hi2c->Instance == I2C4) {
        printf("Slave finished sending: %s\r\n", i2c_slave_tx_data);
    }
}



// Called each time a byte is received
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1 || hi2c->Instance == I2C4){
        slave_rx_i2c_byte_count++;  // increment received byte slave_rx_i2c_byte_count
        if (slave_rx_i2c_byte_count < I2C_BUFF_SIZE)
        {
            // Prepare to receive next byte
            HAL_I2C_Slave_Sequential_Receive_IT(hi2c, &i2c_slave_rx_buffer[slave_rx_i2c_byte_count], 1, I2C_NEXT_FRAME);
        }
        else
        {
            printf("Buffer full, stopping reception\r\n");
        }
    }
}




void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c)
{
	HAL_I2C_EnableListen_IT(hi2c);
}




#endif