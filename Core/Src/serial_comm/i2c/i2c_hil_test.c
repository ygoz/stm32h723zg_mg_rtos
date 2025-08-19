#include "serial_comm/i2c/i2c_hil_test.h"


#ifdef HIL_TEST_MODE


uint8_t rx_buffer[32];  // Buffer to store incoming data


// ---- Start I2C slave receive in interrupt mode ----
HAL_StatusTypeDef I2C1_Start_Receive(void) {
    HAL_StatusTypeDef status = HAL_I2C_Slave_Receive_IT(&hi2c1, rx_buffer, 1);
    if (status != HAL_OK) {
        printf("Failed to start I2C receive, status=%d\n", status);
    }
    else{
        printf("I2C ready to receive, status=%d\n", status);

    }
    return status;
}

// ---- Callback when receive completes ----
void HAL_I2C_SlaveRxCpltCallback(I2C_HandleTypeDef *hi2c) {
    if (hi2c->Instance == I2C1) {
        printf("I2C1 packet received! Triggered interrupt.\n");

        // Optionally, print first few bytes as hex
        // printf("Data: ");
        // for (int i = 0; i < 8 && i < sizeof(rx_buffer); i++) {
        //     printf("%02X ", rx_buffer[i]);
        // }
        // printf("\n");

        // Re-start receive for next packet
        I2C1_Start_Receive();
    }
}

// // ---- Callback for errors ----
// void HAL_I2C_ErrorCallback(I2C_HandleTypeDef *hi2c) {
//     if (hi2c->Instance == I2C1) {
//         printf("I2C1 error occurred! Error code: %lu\n", hi2c->ErrorCode);

//         // Try to restart listening
//         I2C1_Start_Receive();
//     }
// }





#endif