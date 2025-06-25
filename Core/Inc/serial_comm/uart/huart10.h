/**
 * @file huart10.h
 * @author Yam Goz
 * @brief Struct-based UART10 interface with interrupt-driven RX/TX for STM32H723
 * @version 0.2
 * @date 2025-06-24
 * 
 * @details
 * This module provides a modular, struct-based interface for UART10 communication,
 * using interrupts for both RX and TX. It wraps all UART state and operations in a `Uart` struct.
 * 
 * RX behavior:
 * - Byte-by-byte reception via interrupt
 * - Buffer is null-terminated on `\r` or `\n`

 * Configuration macros (in `HaGeneral_config.h`):
 * - `UART10_HANDLE_STATUS`
 * - `UART10_BAUD_RATE`
 * - `UART10_RX_BUFFER_SIZE`

 * Pins:
 * - RX: PE2
 * - TX: PE3
 * 
 * Main elements:
 * - `extern Uart uart10;`
 * - `HAL_StatusTypeDef uart10_init(Uart *uart);`
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
extern Uart uart10;

void MX_USART10_UART_Init(void);


/**
 * @brief Initialize UART10 for interrupt-based reception.
 * @retval HAL_OK on success, HAL_ERROR if UART10 is disabled.
 */
HAL_StatusTypeDef uart10_init(Uart *uart);