/**
 * @file huart8.h
 * @author Yam Goz
 * @brief Struct-based UART8 interface with interrupt-driven RX/TX for STM32H723
 * @version 0.2
 * @date 2025-06-24
 *
 * @details
 * Provides a modular, struct-based UART8 interface with interrupt-based RX and TX.
 * UART state and behavior are encapsulated in a `Uart` struct.
 *
 * RX behavior:
 * - Receives data byte-by-byte via interrupt
 * - Terminates buffer on `\r` or `\n` and sets status to `HAL_OK`
 *
 * Configuration (from `HaGeneral_config.h`):
 * - `UART8_HANDLE_STATUS`
 * - `UART8_BAUD_RATE`
 * - `UART8_RX_BUFFER_SIZE` 
 * - `UART8_CTS_RTS_MODE` (flow control mode)
 * 
 * Pin mapping:
 * - TX: PE1
 * - RX: PE0
 * - RTS: PD15 *(if flow control is enabled)*
 * - CTS: PD14 *(if flow control is enabled)*
 *
 * Main elements:
 * - `extern Uart uart8;`
 * - `HAL_StatusTypeDef uart8_init(Uart *uart);`
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
extern Uart uart8;

void MX_UART8_Init(void);

/**
 * @brief Initialize UART8 for interrupt-based reception.
 * @retval HAL_OK on success, HAL_ERROR if UART8 is disabled.
 */
HAL_StatusTypeDef uart8_init(Uart *uart);
