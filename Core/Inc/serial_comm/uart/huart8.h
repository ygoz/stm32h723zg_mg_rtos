/**
 * @file huart8.h
 * @author Yam Goz
 * @brief STM32H723 UART8 interface — initialization, TX and RX using interrupts
 * @version 0.1
 * @date 2025-06-24
 *
 * @details
 * This module provides initialization and interrupt-based communication functions
 * for the UART8 peripheral on STM32H7 microcontrollers.
 *
 * UART8 is configured to work in **interrupt mode**, where:
 * - RX (receive) interrupt is triggered **per byte received**
 * - TX (transmit) is also handled using interrupts for non-blocking transmission
 *
 * Pin mapping:
 * - **RX**: PE0
 * - **TX**: PE1
 * - **RTS/CTS**: Optional hardware flow control if enabled in `MX_UART8_Init()`
 * - **CTS**: PD14
 * - **RTS**: PD15
 *
 * Configuration macros (defined in `HaGeneral_config.h`):
 * - `UART8_HANDLE_STATUS`: Set to `UART_HANDLE_ON` to enable UART8
 * - `UART8_BAUD_RATE`: Baud rate for communication (e.g., `115200`)
 * - `UART8_RX_BUFFER_SIZE`: Size of RX buffer (e.g., `128`, up to `1024`)
 *
 * Main functions:
 * - `uart8_init()`: Initializes UART8 and begins interrupt-based RX
 * - `uart8_tx()`: Sends a string over UART8 using TX interrupt
 * - `uart8_rx_interrupt_callback()`: Called automatically for each received byte
 *
 * @note Reception continues until `\r` or `\n` is received, then the buffer is null-terminated.
 */

#pragma once

#include "serial_comm/uart/utils.h"

// ============================
// UART8 Configuration Checks
// ============================

// ---------- Handle Status ----------
#ifndef UART8_HANDLE_STATUS
#error "UART8_HANDLE_STATUS is not defined. Please define it in HaGeneral_config.h (e.g., #define UART8_HANDLE_STATUS UART_HANDLE_ON)"
#endif

#if (UART8_HANDLE_STATUS != UART_HANDLE_ON) && \
    (UART8_HANDLE_STATUS != UART_HANDLE_OFF)
#error "Invalid UART8_HANDLE_STATUS. Allowed values: UART_HANDLE_ON or UART_HANDLE_OFF"
#endif

// ---------- RX Buffer Size ----------
#ifndef UART8_RX_BUFFER_SIZE
#error "UART8_RX_BUFFER_SIZE is not defined. Please define it in HaGeneral_config.h (e.g., #define UART8_RX_BUFFER_SIZE 128)"
#endif

#if (UART8_RX_BUFFER_SIZE < 1) || (UART8_RX_BUFFER_SIZE > 1024)
#error "UART8_RX_BUFFER_SIZE must be between 1 and 1024 bytes"
#endif

// ---------- Baud Rate ----------
#ifndef UART8_BAUD_RATE
#error "UART8_BAUD_RATE is not defined. Please define it in HaGeneral_config.h (e.g., #define UART8_BAUD_RATE 115200)"
#endif

#if (UART8_BAUD_RATE < 3000) || (UART8_BAUD_RATE > 3000000)
#error "UART8_BAUD_RATE is out of typical range (3000 to 3,000,000 bps)"
#endif

// ---------- CTS/RTS Mode ----------
#ifndef UART8_CTS_RTS_MODE
#error "UART8_CTS_RTS_MODE is not defined. Please define it in HaGeneral_config.h (e.g., #define UART8_CTS_RTS_MODE UART_CTS_RTS_DISABLED)"
#endif

#if (UART8_CTS_RTS_MODE != UART_CTS_RTS_ENABLED) && \
    (UART8_CTS_RTS_MODE != UART_CTS_RTS_DISABLED)
#error "Invalid UART8_CTS_RTS_MODE. Allowed values: UART_CTS_RTS_ENABLED or UART_CTS_RTS_DISABLED"
#endif

// UART8 Handle and RX Buffer
extern UART_HandleTypeDef huart8;
extern char UART8_RX_BUFFER[UART8_RX_BUFFER_SIZE];

void MX_UART8_Init(void);

/**
 * @brief Initialize UART8 for interrupt-based reception.
 * @retval HAL_OK on success, HAL_ERROR if UART8 is disabled.
 */
HAL_StatusTypeDef uart8_init(void);

/**
 * @brief Transmit a null-terminated string via UART8 using interrupts.
 * @param data Pointer to the string to transmit.
 * @retval HAL_OK on success, HAL_ERROR if input is invalid.
 */
HAL_StatusTypeDef uart8_tx(const char *data);

/**
 * @brief UART8 RX interrupt handler callback.
 * Accumulates data into UART8_RX_BUFFER until newline is received.
 * @retval HAL_OK on successful reception or restart, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef uart8_rx_interrupt_callback(void);
