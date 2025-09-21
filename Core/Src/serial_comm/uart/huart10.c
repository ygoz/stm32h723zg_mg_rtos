#include "serial_comm/uart/huart10.h"
#include "mongoose.h"



UART_HandleTypeDef huart10;
static uint8_t uart10_rx_byte;
static uint16_t uart10_rx_index;
static HAL_StatusTypeDef uart10_rx_status;
static char uart10_rx_buffer[UART10_RX_BUFFER_SIZE];

Uart uart10 = {
    .handle = &huart10,
    .rx_buffer = uart10_rx_buffer,
    .rx_buffer_size = UART10_RX_BUFFER_SIZE,
    .rx_byte = &uart10_rx_byte,
    .rx_index = &uart10_rx_index,
    .rx_status = &uart10_rx_status,
    .init = uart10_init,
    .tx = uart_tx,
    .rx_callback = uart_rx_interrupt_callback,
};



HAL_StatusTypeDef uart10_init(Uart *uart) {
#if UART10_HANDLE_STATUS == UART_HANDLE_ON
    return uart_generic_init(uart);
#else
    MG_INFO(("UART10 handle is defined as OFF!"));
    return HAL_ERROR;
#endif
}


void MX_USART10_UART_Init(void){

  /* USER CODE BEGIN USART10_Init 0 */

  /* USER CODE END USART10_Init 0 */

  /* USER CODE BEGIN USART10_Init 1 */

  /* USER CODE END USART10_Init 1 */
  huart10.Instance = USART10;
  huart10.Init.BaudRate = UART10_BAUD_RATE; // config
  huart10.Init.WordLength = UART_WORDLENGTH_8B;
  huart10.Init.StopBits = UART_STOPBITS_1;
  huart10.Init.Parity = UART_PARITY_NONE;
  huart10.Init.Mode = UART_MODE_TX_RX;
  huart10.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart10.Init.OverSampling = UART_OVERSAMPLING_16;
  huart10.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart10.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  #ifdef HIL_SLAVE_MODE
  huart10.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_SWAP_INIT;
  huart10.AdvancedInit.Swap = UART_ADVFEATURE_SWAP_ENABLE;
  #else
  huart10.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  #endif
  if (HAL_UART_Init(&huart10) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart10, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart10, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART10_Init 2 */

  /* USER CODE END USART10_Init 2 */

}