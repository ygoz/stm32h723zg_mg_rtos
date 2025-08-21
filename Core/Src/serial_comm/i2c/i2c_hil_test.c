#include "main.h"
#include <stdio.h>
#include "HaGeneral/HaGeneral_config.h"
#include "serial_comm/i2c/hi2c1.h"


#ifdef HIL_TEST_MODE


#define I2C_MASTER_REQUEST "shalom from master! can you hear me?"
#define I2C_SLAVE_RESPONSE_PASS "shalom master! this is slave."

#define I2C_RX_BUFF_SIZE  64
uint8_t i2c_rx_buffer[I2C_RX_BUFF_SIZE];

int i2c_byte_count = 0;


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
            i2c_byte_count = 0;  // reset i2c_byte_count for new transfer
            memset(i2c_rx_buffer, 0, I2C_RX_BUFF_SIZE);
            HAL_I2C_Slave_Sequential_Receive_IT(hi2c, &i2c_rx_buffer[i2c_byte_count], 1, I2C_FIRST_FRAME);
        }
        else if (TransferDirection == I2C_DIRECTION_RECEIVE)
        {
            if (strncmp((char*)i2c_rx_buffer, I2C_MASTER_REQUEST, strlen(I2C_MASTER_REQUEST)) == 0) {
                printf("Message matches expected request!\r\n");
                uint8_t tx_data[] = I2C_SLAVE_RESPONSE_PASS;  // the data to send
                HAL_I2C_Slave_Sequential_Transmit_IT(hi2c, tx_data, sizeof(tx_data) - 1, I2C_FIRST_AND_LAST_FRAME);
            } else {
                printf("Message does not match. Expected: \"%s\"\r\n", I2C_MASTER_REQUEST);
            }
        }
    }
}

// Called each time a byte is received
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    if (hi2c->Instance == I2C1 || hi2c->Instance == I2C4){
        i2c_byte_count++;  // increment received byte i2c_byte_count
        // printf("Received byte %d: %u\r\n", i2c_byte_count, i2c_rx_buffer[i2c_byte_count-1]);
        if (i2c_byte_count < I2C_RX_BUFF_SIZE)
        {
            // Prepare to receive next byte
            HAL_I2C_Slave_Sequential_Receive_IT(hi2c, &i2c_rx_buffer[i2c_byte_count], 1, I2C_NEXT_FRAME);
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



// Function to test I2C transmit + receive
HAL_StatusTypeDef hil_test_i2c(I2C_HandleTypeDef *hi2c)
{
    printf("Starting I2C test....\n");

    // ---- TRANSMIT ----
    uint8_t *i2c_buffer = (uint8_t *)I2C_MASTER_REQUEST;
    uint16_t i2c_slave_addr = 0x50;   // shift 7-bit address for HAL

    HAL_StatusTypeDef status;

    // Start non-blocking transmit
    status = HAL_I2C_Master_Transmit_IT(hi2c, i2c_slave_addr, i2c_buffer, strlen(I2C_MASTER_REQUEST));
    if (status != HAL_OK) {
        printf("I2C transmit start failed, status=%d\n", status);
        return status;
    }

    // Wait for completion with timeout
    uint32_t start_tick = HAL_GetTick();
    while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY) {
        if ((HAL_GetTick() - start_tick) > 1000) {
            printf("I2C transmit timed out!\n");
            return HAL_TIMEOUT;
        }
    }

    printf("I2C transmit success\n");

    // ---- RECEIVE ----
    uint8_t rx_buffer[64] = {0};   // adjust size to expected response
    status = HAL_I2C_Master_Receive(hi2c, i2c_slave_addr, rx_buffer, strlen(I2C_SLAVE_RESPONSE_PASS), HAL_MAX_DELAY);

    if (status == HAL_OK) {
        printf("Received from slave: %s\r\n", rx_buffer);
    } else {
        printf("I2C receive error, status=%d\r\n", status);
    }

    printf("Done with I2C test....\n");

    return status;
}




#endif