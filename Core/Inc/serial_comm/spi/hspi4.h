/**
 * @file hspi4.h
 * @author Yam Goz
 * @brief SPI4 interface for STM32H723 with W25Q flash support
 * @version 0.1
 * @date 2025-07-06
 *
 * @details
 * Provides an SPI4 interface for communication with W25Q-series SPI NOR flash memory.
 * Core flash operations (erase, write, read) are encapsulated in a `SPI_Flash` struct,
 * enabling object-like usage with the STM32 HAL SPI driver.
 *
 * Pin mapping for SPI4:
 * - SCK:  PE12
 * - MISO: PE5
 * - MOSI: PE14
 * - NSS:  PE4 *(hardware-controlled chip select)*
 *
 */


#pragma once
#include "stm32h7xx_hal.h"
#include "serial_comm/spi/utils.h"
#include "HaGeneral/HaGeneral_config.h"


// ============================
// SPI4 Configuration Checks
// ============================

// ---------- Clock Polarity ----------
#ifndef SPI4_CLK_POLARITY
#error "SPI4_CLK_POLARITY is not defined. Please define it (e.g., #define SPI4_CLK_POLARITY SPI_POLARITY_LOW)"
#endif

#if (SPI4_CLK_POLARITY != SPI_POLARITY_LOW) && \
    (SPI4_CLK_POLARITY != SPI_POLARITY_HIGH)
#error "Invalid SPI4_CLK_POLARITY. Allowed values: SPI_POLARITY_LOW or SPI_POLARITY_HIGH"
#endif

// ---------- Clock Phase ----------
#ifndef SPI4_CLK_PHASE
#error "SPI4_CLK_PHASE is not defined. Please define it (e.g., #define SPI4_CLK_PHASE SPI_PHASE_1EDGE)"
#endif

#if (SPI4_CLK_PHASE != SPI_PHASE_1EDGE) && \
    (SPI4_CLK_PHASE != SPI_PHASE_2EDGE)
#error "Invalid SPI4_CLK_PHASE. Allowed values: SPI_PHASE_1EDGE or SPI_PHASE_2EDGE"
#endif

// ---------- Baud Rate Prescaler ----------
#ifndef SPI4_BAUDRATE_PRESCALER
#error "SPI4_BAUDRATE_PRESCALER is not defined. Please define it (e.g., #define SPI4_BAUDRATE_PRESCALER SPI_BAUDRATEPRESCALER_32)"
#endif

#if (SPI4_BAUDRATE_PRESCALER != SPI_BAUDRATEPRESCALER_2)   && \
    (SPI4_BAUDRATE_PRESCALER != SPI_BAUDRATEPRESCALER_4)   && \
    (SPI4_BAUDRATE_PRESCALER != SPI_BAUDRATEPRESCALER_8)   && \
    (SPI4_BAUDRATE_PRESCALER != SPI_BAUDRATEPRESCALER_16)  && \
    (SPI4_BAUDRATE_PRESCALER != SPI_BAUDRATEPRESCALER_32)  && \
    (SPI4_BAUDRATE_PRESCALER != SPI_BAUDRATEPRESCALER_64)  && \
    (SPI4_BAUDRATE_PRESCALER != SPI_BAUDRATEPRESCALER_128) && \
    (SPI4_BAUDRATE_PRESCALER != SPI_BAUDRATEPRESCALER_256)
#error "Invalid SPI4_BAUDRATE_PRESCALER. Allowed values: SPI_BAUDRATEPRESCALER_2,4,8,16,32,64,128,256"
#endif

// ---------- NSS Mode ----------
#ifndef SPI4_NSS_MODE
#error "SPI4_NSS_MODE is not defined. Please define it (e.g., #define SPI4_NSS_MODE SPI_NSS_HARD_OUTPUT)"
#endif

#if (SPI4_NSS_MODE != SPI_NSS_HARD_OUTPUT) && \
    (SPI4_NSS_MODE != SPI_NSS_SOFT) && \
    (SPI4_NSS_MODE != SPI_NSS_HARD_INPUT)
#error "Invalid SPI4_NSS_MODE. Allowed values: SPI_NSS_HARD_OUTPUT, SPI_NSS_SOFT, or SPI_NSS_HARD_INPUT"
#endif




extern SPI_HandleTypeDef hspi4;
extern SPI_Flash my_flash;

void MX_SPI4_Init(void);

