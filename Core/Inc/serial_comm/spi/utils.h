/**
 * @file spi_flash_utils.h
 * @author Yam Goz
 * @brief SPI Flash utility driver for W25Q series using STM32 HAL SPI.
 * @version 0.1
 * @date 2025-07-06
 *
 * @copyright
 * Copyright (c) 2025 Your Organization.
 *
 * @details
 * This file provides a high-level interface for working with W25Q-series SPI NOR flash chips,
 * including sector erasing, page programming, and reading data.
 * The API wraps low-level flash commands into a convenient `SPI_Flash` struct with methods
 * that encapsulate SPI communication through STM32 HAL.
 *
 * The library supports:
 * - Erasing 4KB sectors
 * - Writing up to 256-byte pages
 * - Reading arbitrary lengths from flash memory
 * - Object-like usage with `SPI_Flash` instances
 *
 * Example usage:
 * @code
 * SPI_Flash my_flash = {
 *     .hspi = &hspi4,
 *     .erase_sector = flash_erase_sector,
 *     .write_page = flash_write_page,
 *     .read = flash_read
 * };
 * 
 * uint8_t data[] = "Hello SPI Flash!";
 * uint8_t readback[sizeof(data)] = {0};
 * 
 * my_flash.erase_sector(&my_flash, 0x000000);
 * my_flash.write_page(&my_flash, 0x000000, data, sizeof(data));
 * my_flash.read(&my_flash, 0x000000, readback, sizeof(readback));
 * @endcode
 */


#pragma once


#include <stm32h7xx_hal_spi.h>
#include <string.h>
#include "stm32h7xx_hal.h"



#define CMD_WRITE_ENABLE   0x06
#define CMD_SECTOR_ERASE   0x20
#define CMD_PAGE_PROGRAM   0x02
#define CMD_READ_DATA      0x03
#define CMD_READ_STATUS    0x05
#define SPI_TIMEOUT        100 // 100 ms timeout
#define W25Q_MAX_ADDR      0xFFFFFF // 16 MB address space (24 bits)
#define W25Q_MAX_READ_LEN  65535 // Max read length (uint16_t max)


#define LOG_INFO(...) printf(__VA_ARGS__)

typedef struct SPI_Flash {
    SPI_HandleTypeDef *hspi;   // SPI handle used for this Flash instance

    // Core methods:
    HAL_StatusTypeDef (*erase_sector)(struct SPI_Flash *self, uint32_t addr);
    HAL_StatusTypeDef (*write_page)(struct SPI_Flash *self, uint32_t addr, uint8_t *data, uint16_t len);
    HAL_StatusTypeDef (*read)(struct SPI_Flash *self, uint32_t addr, uint8_t *rx, uint16_t len);
} SPI_Flash;


/**
 * @brief Erases a 4KB sector at the specified address in the SPI flash.
 *
 * This function calls the low-level flash erase implementation using the SPI handle
 * stored in the SPI_Flash object. The address must be aligned to the sector size (4KB).
 *
 * @param self Pointer to the SPI_Flash instance.
 * @param addr 24-bit address of the sector to erase (must be sector-aligned).
 * @return HAL_OK on success, or an error code (HAL_ERROR/HAL_TIMEOUT).
 */
HAL_StatusTypeDef flash_erase_sector(SPI_Flash *self, uint32_t addr);

/**
 * @brief Writes data to a single page (up to 256 bytes) in the SPI flash.
 *
 * This function writes data starting at the specified page-aligned address. It will call
 * the flash page program command through the SPI handle in the SPI_Flash object.
 *
 * @param self Pointer to the SPI_Flash instance.
 * @param addr 24-bit page start address (must be page-aligned).
 * @param data Pointer to the data buffer to write.
 * @param len Length of data to write (max 256 bytes).
 * @return HAL_OK on success, or an error code (HAL_ERROR/HAL_TIMEOUT).
 */
HAL_StatusTypeDef flash_write_page(SPI_Flash *self, uint32_t addr, uint8_t *data, uint16_t len);

/**
 * @brief Reads data from the specified address in the SPI flash into a user buffer.
 *
 * This function sends the read command with a 24-bit address and reads `len` bytes
 * of data into the provided buffer using the SPI handle in the SPI_Flash object.
 *
 * @param self Pointer to the SPI_Flash instance.
 * @param addr 24-bit start address to read from.
 * @param rx Pointer to the buffer where read data will be stored.
 * @param len Number of bytes to read.
 * @return HAL_OK on success, or an error code (HAL_ERROR/HAL_TIMEOUT).
 */
HAL_StatusTypeDef flash_read(SPI_Flash *self, uint32_t addr, uint8_t *rx, uint16_t len);



