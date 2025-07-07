/**
 * @file hspi5.h
 * @author Yam Goz
 * @brief SPI5 interface for STM32H723 with W25Q flash support
 * @version 0.1
 * @date 2025-07-06
 *
 * @details
 * Provides an SPI5 interface for communication with W25Q-series SPI NOR flash memory.
 * Core flash operations (erase, write, read) are encapsulated in a `SPI_Flash` struct,
 * enabling object-like usage with the STM32 HAL SPI driver.
 *
 * Pin mapping for SPI5:
 * - SCK:  PF7
 * - MISO: PF8
 * - MOSI: PF9
 * - NSS:  PF6 *(hardware-controlled chip select)*
 *
 */


#pragma once
#include "stm32h7xx_hal.h"
#include "serial_comm/spi/utils.h"
#include "HaGeneral/HaGeneral_config.h"


// ============================
// SPI5 Configuration Checks
// ============================

// ---------- Clock Polarity ----------
#ifndef SPI5_CLK_POLARITY
#error "SPI5_CLK_POLARITY is not defined. Please define it (e.g., #define SPI5_CLK_POLARITY SPI_POLARITY_LOW)"
#endif

#if (SPI5_CLK_POLARITY != SPI_POLARITY_LOW) && \
    (SPI5_CLK_POLARITY != SPI_POLARITY_HIGH)
#error "Invalid SPI5_CLK_POLARITY. Allowed values: SPI_POLARITY_LOW or SPI_POLARITY_HIGH"
#endif

// ---------- Clock Phase ----------
#ifndef SPI5_CLK_PHASE
#error "SPI5_CLK_PHASE is not defined. Please define it (e.g., #define SPI5_CLK_PHASE SPI_PHASE_1EDGE)"
#endif

#if (SPI5_CLK_PHASE != SPI_PHASE_1EDGE) && \
    (SPI5_CLK_PHASE != SPI_PHASE_2EDGE)
#error "Invalid SPI5_CLK_PHASE. Allowed values: SPI_PHASE_1EDGE or SPI_PHASE_2EDGE"
#endif

// ---------- Baud Rate Prescaler ----------
#ifndef SPI5_BAUDRATE_PRESCALER
#error "SPI5_BAUDRATE_PRESCALER is not defined. Please define it (e.g., #define SPI5_BAUDRATE_PRESCALER SPI_BAUDRATEPRESCALER_32)"
#endif

#if (SPI5_BAUDRATE_PRESCALER != SPI_BAUDRATEPRESCALER_2)   && \
    (SPI5_BAUDRATE_PRESCALER != SPI_BAUDRATEPRESCALER_4)   && \
    (SPI5_BAUDRATE_PRESCALER != SPI_BAUDRATEPRESCALER_8)   && \
    (SPI5_BAUDRATE_PRESCALER != SPI_BAUDRATEPRESCALER_16)  && \
    (SPI5_BAUDRATE_PRESCALER != SPI_BAUDRATEPRESCALER_32)  && \
    (SPI5_BAUDRATE_PRESCALER != SPI_BAUDRATEPRESCALER_64)  && \
    (SPI5_BAUDRATE_PRESCALER != SPI_BAUDRATEPRESCALER_128) && \
    (SPI5_BAUDRATE_PRESCALER != SPI_BAUDRATEPRESCALER_256)
#error "Invalid SPI5_BAUDRATE_PRESCALER. Allowed values: SPI_BAUDRATEPRESCALER_2,4,8,16,32,64,128,256"
#endif

// ---------- NSS Mode ----------
#ifndef SPI5_NSS_MODE
#error "SPI5_NSS_MODE is not defined. Please define it (e.g., #define SPI5_NSS_MODE SPI_NSS_HARD_OUTPUT)"
#endif

#if (SPI5_NSS_MODE != SPI_NSS_HARD_OUTPUT) && \
    (SPI5_NSS_MODE != SPI_NSS_SOFT) && \
    (SPI5_NSS_MODE != SPI_NSS_HARD_INPUT)
#error "Invalid SPI5_NSS_MODE. Allowed values: SPI_NSS_HARD_OUTPUT, SPI_NSS_SOFT, or SPI_NSS_HARD_INPUT"
#endif




extern SPI_HandleTypeDef hspi5;
// extern SPI_Flash my_flash;

void MX_SPI5_Init(void);

