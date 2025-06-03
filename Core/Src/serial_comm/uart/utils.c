#include "serial_comm/uart/utils.h"
#include "mongoose.h"





HAL_StatusTypeDef uart_init(char * uart_rx_buffer, uint16_t uart_rx_buffer_len, UART_HandleTypeDef *huart, uint8_t *uart_rx_byte) {
    HAL_StatusTypeDef status = HAL_OK;
    memset(uart_rx_buffer, 0, uart_rx_buffer_len);

    status = HAL_UART_Receive_IT(huart, uart_rx_byte, 1);
    return status;
}


HAL_StatusTypeDef uart_tx(UART_HandleTypeDef *huart, const char *data) {
    if (huart == NULL || data == NULL) return HAL_ERROR;

    uint16_t len = (uint16_t)strlen(data);
    if (len == 0) return HAL_ERROR;

    return HAL_UART_Transmit_IT(huart, (uint8_t *)data, len);
}



HAL_StatusTypeDef uart_rx_interrupt_callback(
    char *rx_buffer,
    uint8_t *rx_byte,
    size_t *rx_index,
    size_t buffer_size,
    HAL_StatusTypeDef (*restart_rx_fn)(void),
    HAL_StatusTypeDef *rx_status
) {
    if (*rx_index < (buffer_size - 1)) {
        rx_buffer[(*rx_index)++] = (char)(*rx_byte);

        if (*rx_byte == '\r' || *rx_byte == '\n') {
            rx_buffer[*rx_index] = '\0';  // Null-terminate
            *rx_status = HAL_OK;
            return HAL_OK;
        } else {
            *rx_status = restart_rx_fn();
            return *rx_status;
        }
    } else {
        // Buffer overflow
        *rx_index = 0;
        memset(rx_buffer, 0, buffer_size);
        *rx_status = restart_rx_fn();
        return *rx_status;
    }
}





void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART10) {
        // MG_INFO(("USART10 RX INTERRUPT"));
        uart10_rx_interrupt_callback();
    } 
    else if (huart->Instance == UART8) {
        uart8_rx_interrupt_callback();
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART10) {
        // MG_INFO(("USART10 TX INTERRUPT"));
    }
}