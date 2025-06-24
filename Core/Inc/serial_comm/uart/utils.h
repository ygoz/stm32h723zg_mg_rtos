/**
 * @file utils.h
 * @author Yam Goz
 * @brief UART Utility Functions for STM32 Projects
 * @version 0.1
 * @date 2025-06-24
 * 
 * @details
 * This file provides a set of generic and reusable utility functions for 
 * UART peripheral handling in STM32-based applications using the HAL library.
 * It includes:
 * 
 * - Initialization helpers
 * - Interrupt-based receive and transmit functions
 * - Common receive buffer handling logic (per-byte interrupt reception)
 * 
 * These utilities allow for flexible integration of multiple UART instances
 * (e.g., UART8, UART10) by decoupling the buffer, byte, and handler context.
 * 
 * 
 * @note All functions rely on non-blocking interrupt-based HAL API.
 * 
 * @attention
 * Ensure that your buffers and byte placeholders are defined statically or persistently
 * in scope and that `restart_rx_fn()` is correctly implemented per UART instance.
 */


#pragma once


#include "stm32h7xx_hal.h"
#include <string.h>
#include <stm32h723xx.h>
#include "HaGeneral/HaGeneral_config.h"



#define UART_HANDLE_OFF 0
#define UART_HANDLE_ON  1

#define UART_CTS_RTS_ENABLED  2
#define UART_CTS_RTS_DISABLED  3


/**
 * @brief Initializes UART reception using interrupt mode.
 *
 * This function prepares the UART to receive data one byte at a time using interrupts.
 * It clears the provided receive buffer and starts listening for the first byte.
 *
 * @param[in,out] uart_rx_buffer      Pointer to the buffer used for storing received UART data.
 * @param[in]     uart_rx_buffer_len  Length of the receive buffer in bytes.
 * @param[in]     huart               Pointer to the UART handle 
 * @param[out]    uart_rx_byte        Pointer to a single-byte storage for interrupt reception.
 *
 * @return HAL_StatusTypeDef
 *         - HAL_OK: Initialization and first byte reception started successfully.
 *         - HAL_ERROR or other: If the UART failed to start in interrupt mode.
 *
 * @note This function must be followed by a custom RX complete interrupt callback handler
 *       that continues reception or processes received data.
 *
 * @warning Ensure that uart_rx_byte and uart_rx_buffer are valid and non-NULL before calling.
 */
HAL_StatusTypeDef uart_init(char * uart_rx_buffer, uint16_t uart_rx_buffer_len, UART_HandleTypeDef *huart, uint8_t *uart_rx_byte);



/**
 * @brief Transmits a string over UART using interrupt mode.
 *
 * This function sends a null-terminated string over the specified UART peripheral
 * using non-blocking interrupt-based transmission (`HAL_UART_Transmit_IT`).
 *
 * @param[in] huart Pointer to the UART handle (e.g., &huart10).
 * @param[in] data  Pointer to the null-terminated string to transmit.
 *
 * @return HAL_StatusTypeDef
 *         - HAL_OK on successful transmission start.
 *         - HAL_ERROR if the input parameters are invalid or the string is empty.
 *
 * @note The UART must be initialized before calling this function.
 *       An appropriate TX complete callback (e.g., `HAL_UART_TxCpltCallback`) should be implemented
 *       to handle post-transmission logic.
 *
 * @warning The input string must remain valid until the transmission completes.
 */
HAL_StatusTypeDef uart_tx(UART_HandleTypeDef *huart, const char *data);


/**
 * @brief Generic UART RX interrupt callback handler.
 *
 * This function is designed to be called within the UART receive complete interrupt context.
 * It accumulates received characters into a buffer until a line-ending character (`\r` or `\n`)
 * is encountered or the buffer is full. If a line-ending is received, the buffer is null-terminated
 * and the receive status is set to `HAL_OK`.
 *
 * @param[in,out] rx_buffer     Pointer to the character buffer used for accumulating received data.
 * @param[in]     rx_byte       Pointer to the received byte (1-byte buffer).
 * @param[in,out] rx_index      Pointer to the current write index in the rx_buffer.
 * @param[in]     buffer_size   Total size of the rx_buffer.
 * @param[in]     restart_rx_fn Function pointer to restart the UART receive interrupt (should receive 1 byte).
 * @param[out]    rx_status     Pointer to status variable to reflect the RX process result.
 *
 * @retval HAL_OK      Successfully received a full line (terminated with `\r` or `\n`).
 * @retval HAL_ERROR   Restart RX failed, or parameters invalid (as set by restart function).
 *
 * @note If buffer overflows before a newline, the buffer is cleared and reception restarts.
 * @note This function is protocol-agnostic and can be reused for multiple UART instances.
 *
 * @warning Ensure the rx_buffer has enough space (at least buffer_size) and all pointers are valid.
 * @warning The `restart_rx_fn` must correctly re-arm the UART for the next byte (e.g., via `HAL_UART_Receive_IT`).
 */
HAL_StatusTypeDef uart_rx_interrupt_callback(
    char *rx_buffer,
    uint8_t *rx_byte,
    size_t *rx_index,
    size_t buffer_size,
    HAL_StatusTypeDef (*restart_rx_fn)(void),
    HAL_StatusTypeDef *rx_status
);