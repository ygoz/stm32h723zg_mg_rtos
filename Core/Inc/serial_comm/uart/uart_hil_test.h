#pragma once


#include "stm32h7xx_hal.h"
#include "HaGeneral/HaGeneral_config.h"
#include "serial_comm/uart/utils.h"


#ifdef HIL_TEST_MODE


/**
 * @brief Default test message sent from master to slave.
 */
#define UART_MASTER_REQUEST      "shalom from master! can you hear me?\n"


/**
 * @brief Default response message sent by the UART slave
 *        when a valid master request is received.
 */
#define UART_SLAVE_RESPONSE      "shalom master! this is slave.\n"


#ifdef HIL_MASTER_MODE


HAL_StatusTypeDef hil_test_uart(Uart *uart);


#endif 



#ifdef HIL_SLAVE_MODE


HAL_StatusTypeDef uart_slave_init();


#endif


#endif