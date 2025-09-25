#include "main.h"
#include <stdio.h>
#include "HaGeneral/HaGeneral_config.h"
#include "serial_comm/uart/huart8.h"
#include "serial_comm/uart/huart10.h"
#include "serial_comm/uart/utils.h"
#include "serial_comm/uart/uart_hil_test.h"


#ifdef HIL_SLAVE_MODE


#include "string.h"



static HAL_StatusTypeDef restart_rx(Uart *uart) {
    return HAL_UART_Receive_IT(uart->handle, uart->rx_byte, 1);
}



static HAL_StatusTypeDef process_uart_data(Uart *uart)
{
    if (strncmp(uart->rx_buffer, UART_MASTER_REQUEST, strlen(UART_MASTER_REQUEST)) == 0) {
        return uart->tx(uart, UART_SLAVE_RESPONSE);
    }
    else {
        return uart->tx(uart, "NACK\0");
    }
}





HAL_StatusTypeDef uart_slave_rx_interrupt_callback(Uart *uart) {
    if (!uart) return HAL_ERROR;

    if (*(uart->rx_index) < uart->rx_buffer_size - 1) {
        uart->rx_buffer[(*(uart->rx_index))++] = *(uart->rx_byte);

        // look for termination, either \r or \n
        if (*(uart->rx_byte) == '\r' || *(uart->rx_byte) == '\n') {

            // send back data here if the request was recieved
            if (process_uart_data(uart) == HAL_OK) {
                printf("\r\nSending back data on UART\r\n");
            } else {
                printf("\r\ncould NOT send back data on UART\r\n");
            }
            uart->init(uart);
        } else {
            // keep listening for the next byte
            *(uart->rx_status) = restart_rx(uart);
        }
    } else {
        // Buffer overflow, reset handle
        return uart->init(uart);
    }

    return *(uart->rx_status);
}






// Call this once in your init code
HAL_StatusTypeDef uart_slave_init()
{
    // Start listening for data from the master
    uart10.rx_callback = uart_slave_rx_interrupt_callback;
    if (uart10.init(&uart10) != HAL_OK) {
        return HAL_ERROR;
    }

    uart8.rx_callback = uart_slave_rx_interrupt_callback;
    if (uart8.init(&uart8) != HAL_OK) {
        return HAL_ERROR;
    }
    return HAL_OK;
}









#endif