#include "serial_comm/uart/utils.h"
#include "mongoose.h"
#include "serial_comm/uart/huart10.h"
#include "serial_comm/uart/huart8.h"


static HAL_StatusTypeDef restart_rx(Uart *uart) {
    return HAL_UART_Receive_IT(uart->handle, uart->rx_byte, 1);
}


HAL_StatusTypeDef uart_generic_init(Uart *uart) {
    if (!uart) return HAL_ERROR;

    memset(uart->rx_buffer, 0, uart->rx_buffer_size);
    *uart->rx_index = 0;
    *uart->rx_status = HAL_OK;

    return restart_rx(uart);
}


HAL_StatusTypeDef uart_tx(Uart *uart, const char *data) {
    if (!uart || !data) return HAL_ERROR;

    size_t len = strlen(data);
    if (len == 0) return HAL_ERROR;

    return HAL_UART_Transmit_IT(uart->handle, (uint8_t *)data, len);
}



HAL_StatusTypeDef uart_rx_interrupt_callback(Uart *uart) {
    if (!uart) return HAL_ERROR;

    if (*(uart->rx_index) < uart->rx_buffer_size - 1) {
        uart->rx_buffer[(*(uart->rx_index))++] = *(uart->rx_byte);

        // look for termination, either \r or \n
        if (*(uart->rx_byte) == '\r' || *(uart->rx_byte) == '\n') {
            // set null termination
            uart->rx_buffer[*(uart->rx_index)] = '\0';
            *(uart->rx_status) = HAL_OK;
        } else {
            // keep listening for the next byte
            *(uart->rx_status) = restart_rx(uart);
        }
    } else {
        // Buffer overflow
        //reset index
        *(uart->rx_index) = 0;
        // clear buff
        memset(uart->rx_buffer, 0, uart->rx_buffer_size);
        *(uart->rx_status) = restart_rx(uart);
    }

    return *(uart->rx_status);
}





void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == UART8) {
        uart8.rx_callback(&uart8);
    } else if (huart->Instance == USART10) {
        uart10.rx_callback(&uart10);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART10) {
        // MG_INFO(("USART10 TX INTERRUPT"));
    }
}