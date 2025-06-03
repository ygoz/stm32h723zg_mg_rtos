#include "serial_comm/uart/huart8.h"
#include "mongoose.h"

UART_HandleTypeDef huart8;
static uint8_t uart8_rx_byte;
static uint16_t uart8_rx_index = 0;
static volatile HAL_StatusTypeDef uart8_rx_status = HAL_OK;
char UART8_RX_BUFFER[UART8_RX_BUFFER_SIZE];

static HAL_StatusTypeDef uart8_restart_rx(void) {
    return HAL_UART_Receive_IT(&huart8, &uart8_rx_byte, 1);
}

HAL_StatusTypeDef uart8_init(void) {
    #if UART8_HANDLE_STATUS == UART_HANDLE_ON
    return uart_init(
        UART8_RX_BUFFER,
        UART8_RX_BUFFER_SIZE,
        &huart8,
        &uart8_rx_byte
    );
    #else
    MG_INFO(("uart8 handle is defined as OFF!"));
    return HAL_ERROR;
    #endif
}

HAL_StatusTypeDef uart8_rx_interrupt_callback(void) {
    return uart_rx_interrupt_callback(
        UART8_RX_BUFFER,
        &uart8_rx_byte,
        &uart8_rx_index,
        UART8_RX_BUFFER_SIZE,
        uart8_restart_rx,
        &uart8_rx_status
    );
}

HAL_StatusTypeDef uart8_tx(const char *data) {
    return uart_tx(&huart8, data);
}

/**
  * @brief UART8 Initialization Function
  * @param None
  * @retval None
  */
void MX_UART8_Init(void){

  /* USER CODE BEGIN UART8_Init 0 */

  /* USER CODE END UART8_Init 0 */

  /* USER CODE BEGIN UART8_Init 1 */

  /* USER CODE END UART8_Init 1 */
  huart8.Instance = UART8;
  huart8.Init.BaudRate = UART8_BAUD_RATE; // config
  huart8.Init.WordLength = UART_WORDLENGTH_8B;
  huart8.Init.StopBits = UART_STOPBITS_1;
  huart8.Init.Parity = UART_PARITY_NONE;
  huart8.Init.Mode = UART_MODE_TX_RX;
  #if UART8_CTS_RTS_MODE == UART_CTS_RTS_ENABLED
  huart8.Init.HwFlowCtl = UART_HWCONTROL_RTS_CTS; // config
  #elif UART8_CTS_RTS_MODE == UART_CTS_RTS_DISABLED
  huart8.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  #endif
  huart8.Init.OverSampling = UART_OVERSAMPLING_16;
  huart8.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart8.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart8.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart8, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart8, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart8) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART8_Init 2 */

  /* USER CODE END UART8_Init 2 */

}