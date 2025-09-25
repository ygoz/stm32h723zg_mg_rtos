#include "HaGeneral/HaGeneral_config.h"



#ifdef HIL_MASTER_MODE


#include "main.h"
#include <stdio.h>
#include "serial_comm/uart/huart8.h"
#include "serial_comm/uart/huart10.h"
#include "serial_comm/uart/utils.h"
#include "serial_comm/uart/uart_hil_test.h"




// Call this to start the UART test
HAL_StatusTypeDef hil_test_uart(Uart *uart)
{
    uart->init(uart);
    // Start TX/RX
    HAL_StatusTypeDef status = uart->tx(uart, UART_MASTER_REQUEST);
    if (status != HAL_OK) {
        printf("uart transmition failed: %d\r\n", status);
        return status;
    }

    // Expecting slave response
    HAL_Delay(100);
    if (strncmp(uart->rx_buffer, UART_SLAVE_RESPONSE, strlen(UART_SLAVE_RESPONSE)) == 0) return HAL_OK;
    else return HAL_ERROR;
}


#endif