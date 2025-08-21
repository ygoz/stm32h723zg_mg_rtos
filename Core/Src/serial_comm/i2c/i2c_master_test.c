#include "main.h"
#include <stdio.h>
#include "HaGeneral/HaGeneral_config.h"
#include "serial_comm/i2c/hi2c1.h"
#include "serial_comm/i2c/hi2c4.h"
#include "serial_comm/i2c/i2c_master_test.h"
#include "serial_comm/i2c/i2c_slave_test.h"

#ifdef HIL_MASTER_MODE



static uint8_t i2c_master_rx_buffer[I2C_BUFF_SIZE] = {0};   // adjust size to expected response




void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
    printf("Master received: %s\n", i2c_master_rx_buffer);
}



HAL_StatusTypeDef wait_for_i2c(I2C_HandleTypeDef *hi2c, uint16_t timeout){
    uint32_t start_tick = HAL_GetTick();
    while (HAL_I2C_GetState(hi2c) != HAL_I2C_STATE_READY) {
        if ((HAL_GetTick() - start_tick) > timeout) {
            printf("I2C transmit timed out!\n");
            return HAL_TIMEOUT;
        }
    }
    return HAL_OK;
}




// Function to test I2C transmit + receive
HAL_StatusTypeDef hil_test_i2c(I2C_HandleTypeDef *hi2c)
{
    memset(i2c_master_rx_buffer, 0, I2C_BUFF_SIZE); // clean rx buffer for new transfer

    printf("Starting I2C test....\n");

    // ---- TRANSMIT ----
    HAL_StatusTypeDef status;
    uint8_t *i2c_tx_data = I2C_MASTER_REQUEST;
    uint16_t i2c_slave_addr = I2C_SLAVE_ADDR;

    // Start non-blocking transmit
    status = HAL_I2C_Master_Seq_Transmit_IT(
        hi2c,
        i2c_slave_addr,
        i2c_tx_data,
        strlen(I2C_MASTER_REQUEST),
        I2C_FIRST_AND_LAST_FRAME  // generate start+stop automatically
    );
    if (status != HAL_OK) {
        printf("I2C transmit start failed, status=%d\n", status);
        return status;
    }

    // Wait for completion with timeout
    status = wait_for_i2c(hi2c, 1000);
    if (status != HAL_OK) {
        return status;
    }

    printf("I2C transmit success\n");

    // ---- RECEIVE ----
    status = HAL_I2C_Master_Seq_Receive_IT(
        hi2c, i2c_slave_addr, i2c_master_rx_buffer, 
        strlen(I2C_SLAVE_RESPONSE_PASS), 
        I2C_FIRST_AND_LAST_FRAME);

    status = wait_for_i2c(hi2c, 1000);
    if (status != HAL_OK) {
        return status;
    }

    if (status == HAL_OK &&
        strncmp((char*)i2c_master_rx_buffer, I2C_SLAVE_RESPONSE_PASS, strlen(I2C_SLAVE_RESPONSE_PASS)) != 0) {
            status = HAL_ERROR;
    }

    return status;
}




#endif