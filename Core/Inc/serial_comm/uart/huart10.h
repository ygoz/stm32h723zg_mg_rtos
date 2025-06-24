/**
 * @file huart10.h
 * @author Yam Goz
 * @brief STM32H723 UART10 interface — initialization, TX and RX using interrupts
 * @version 0.1
 * @date 2025-06-24
 * 
 * @details
 * This module provides initialization and interrupt-based communication functions
 * for the UART10 peripheral on STM32H7 microcontrollers.
 * 
 * UART10 is configured to work in **interrupt mode**, where:
 * - RX (receive) interrupt is triggered **per byte received**
 * - TX (transmit) is also handled using interrupts for non-blocking transmission
 * 
 * Pin mapping:
 * - **RX**: PE2
 * - **TX**: PE3
 * 
 * Configuration macros (defined in `HaGeneral_config.h`):
 * - `UART10_HANDLE_STATUS`: Set to `UART_HANDLE_ON` to enable UART10
 * - `UART10_BAUD_RATE`: Baud rate for communication (e.g., `115200`)
 * - `UART10_RX_BUFFER_SIZE`: Size of RX buffer (e.g., `128`, up to `1024`)
 * 
 * Main functions:
 * - `uart10_init()`: Initializes UART10 and begins interrupt-based RX
 * - `uart10_tx()`: Sends a string over UART10 using TX interrupt
 * - `uart10_rx_interrupt_callback()`: Called automatically for each received byte
 * 
 * @note Reception continues until `\r` or `\n` is received, then the buffer is null-terminated.
 */

#pragma once

#include "serial_comm/uart/utils.h"



// ============================
// UART10 Configuration Checks
// ============================

// ---------- Handle Status ----------
#ifndef UART10_HANDLE_STATUS
#error "UART10_HANDLE_STATUS is not defined. Please define it in HaGeneral_config.h (e.g., #define UART10_HANDLE_STATUS UART_HANDLE_ON)"
#endif

#if (UART10_HANDLE_STATUS != UART_HANDLE_ON) && \
    (UART10_HANDLE_STATUS != UART_HANDLE_OFF)
#error "Invalid UART10_HANDLE_STATUS. Allowed values: UART_HANDLE_ON or UART_HANDLE_OFF"
#endif

// ---------- RX Buffer Size ----------
#ifndef UART10_RX_BUFFER_SIZE
#error "UART10_RX_BUFFER_SIZE is not defined. Please define it in HaGeneral_config.h (e.g., #define UART10_RX_BUFFER_SIZE 128)"
#endif

#if (UART10_RX_BUFFER_SIZE < 1) || (UART10_RX_BUFFER_SIZE > 1024)
#error "UART10_RX_BUFFER_SIZE must be between 1 and 1024 bytes"
#endif

// ---------- Baud Rate ----------
#ifndef UART10_BAUD_RATE
#error "UART10_BAUD_RATE is not defined. Please define it in HaGeneral_config.h (e.g., #define UART10_BAUD_RATE 115200)"
#endif

#if (UART10_BAUD_RATE < 3000) || (UART10_BAUD_RATE > 3000000)
#error "UART10_BAUD_RATE is out of typical range (3000 to 3,000,000 bps)"
#endif




extern UART_HandleTypeDef huart10;
extern char UART10_RX_BUFFER[UART10_RX_BUFFER_SIZE];

void MX_USART10_UART_Init(void);


/**
 * @brief Initialize UART10 for interrupt-based reception.
 * @retval HAL_OK on success, HAL_ERROR if UART10 is disabled.
 */
HAL_StatusTypeDef uart10_init(void);


/**
 * @brief Transmit a null-terminated string via UART10 using interrupts.
 * @param data Pointer to the string to transmit.
 * @retval HAL_OK on success, HAL_ERROR if input is invalid.
 */
HAL_StatusTypeDef uart10_tx(const char *data);


/**
 * @brief UART10 RX interrupt handler callback.
 * Accumulates data into UART10_RX_BUFFER until newline is received.
 * @retval HAL_OK on successful reception or restart, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef uart10_rx_interrupt_callback(void);