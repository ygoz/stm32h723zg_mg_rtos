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
 * These utilities allow for integration of multiple UART instances
 * (e.g., UART8, UART10) by decoupling the buffer, byte, and handler context.
 * 
 * 
 * @note All functions rely on non-blocking interrupt-based HAL API.
 * 
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



typedef struct Uart Uart;


/**
 * @brief UART interface struct for interrupt-based communication.
 *
 * @details
 * Encapsulates all necessary state and function pointers for managing
 * interrupt-driven UART transmission and reception. This struct enables
 * modular, reusable UART interfaces across multiple peripherals (e.g., UART8, UART10).
 *
 * Fields:
 * - UART_HandleTypeDef *handle: HAL UART handle for hardware control.
 * - char *rx_buffer: Pointer to the circular buffer for storing received data.
 * - uint16_t rx_buffer_size: Size of the RX buffer.
 * - uint8_t *rx_byte: Temporary single-byte storage for receiving one byte via interrupt.
 * - uint16_t *rx_index: Pointer to current write index in the RX buffer.
 * - HAL_StatusTypeDef *rx_status: Pointer to receive status flag (set when line terminator is received).
 *
 * Function Pointers:
 * - init: Initializes the UART (typically sets up RX interrupts).
 * - tx: Transmits a null-terminated string using interrupt mode.
 * - rx_callback: Handles RX complete interrupts, appending data and restarting RX.
 */
struct Uart {
    // uart handle
    UART_HandleTypeDef *handle;

    // rx buffer
    char *rx_buffer;
    uint16_t rx_buffer_size;
    uint8_t *rx_byte;
    uint16_t *rx_index;
    HAL_StatusTypeDef *rx_status;

    // helper methods
    HAL_StatusTypeDef (*init)(Uart *self);
    HAL_StatusTypeDef (*tx)(Uart *self, const char *data);
    HAL_StatusTypeDef (*rx_callback)(Uart *self);
};



/**
 * @brief Initializes UART reception using interrupt mode (struct-based).
 *
 * @details
 * Prepares the specified UART instance to receive data one byte at a time using interrupts.
 * It clears the RX buffer, resets the index and status, and starts listening for the first byte.
 *
 * @param[in,out] uart Pointer to a valid Uart struct containing the RX buffer, index, status, and UART handle.
 *
 * @return HAL_StatusTypeDef
 *         - HAL_OK: Initialization and first-byte reception started successfully.
 *         - HAL_ERROR or other: UART handle is NULL or interrupt setup failed.
 *
 * @warning All Uart struct fields must be properly initialized before calling.
 */
HAL_StatusTypeDef uart_generic_init(Uart *uart);


/**
 * @brief Transmits a string over UART using interrupt mode (struct-based).
 *
 * Sends a null-terminated string over the specified UART using non-blocking
 * interrupt-driven transmission via `HAL_UART_Transmit_IT`.
 *
 * @param[in] uart Pointer to a valid Uart struct containing the UART handle.
 * @param[in] data Pointer to the null-terminated string to transmit.
 *
 * @return HAL_StatusTypeDef
 *         - HAL_OK: Transmission started successfully.
 *         - HAL_ERROR: Invalid input or empty string.
 *
 * @note 
 * - The UART must be initialized before calling this function.
 * - Implement `HAL_UART_TxCpltCallback()` to handle post-transmission events if needed.
 *
 * @warning The input string must remain valid in memory until transmission completes.
 */
HAL_StatusTypeDef uart_tx(Uart *uart, const char *data);


/**
 * @brief Generic UART RX interrupt callback handler (struct-based).
 *
 * @details
 * Handles per-byte UART reception using interrupts. Appends each received byte
 * to the RX buffer in the given `Uart` struct. If a line-ending character (`\r` or `\n`)
 * is received, the buffer is null-terminated and the RX status is set to `HAL_OK`.
 * If the buffer overflows before a newline, it is cleared and RX is restarted.
 *
 * @param[in,out] uart Pointer to a valid Uart struct containing RX state and buffer info.
 *
 * @retval HAL_OK: Reception succeeded or restarted.
 * @retval HAL_ERROR: RX restart failed or invalid input.
 *
 * @note Designed for use inside `HAL_UART_RxCpltCallback()`.
 *       Works with any UART instance using a properly initialized `Uart` struct.
 *
 * @warning The `Uart` struct fields must be valid and initialized before calling.
 *          Particularly: `.handle`, `.rx_buffer`, `.rx_byte`, `.rx_index`, `.rx_status`.
 */
HAL_StatusTypeDef uart_rx_interrupt_callback(Uart *uart);