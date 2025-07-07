#include "serial_comm/spi/hspi4.h"
#include "serial_comm/spi/hspi5.h"
#include "serial_comm/spi/utils.h"




/**
 * @brief Send Write Enable command (0x06).
 * @return HAL_OK on success, HAL_ERROR on failure.
 */
HAL_StatusTypeDef w25q_write_enable(SPI_HandleTypeDef *hspi) {
    uint8_t cmd = CMD_WRITE_ENABLE;
    HAL_StatusTypeDef status = HAL_SPI_Transmit(hspi, &cmd, 1, SPI_TIMEOUT);
    if (status != HAL_OK) {
        LOG_INFO("Write Enable failed: %d\r\n", status);
    }
    return status;
}

/**
 * @brief Wait until Flash is not busy (WIP bit = 0).
 * @return HAL_OK on success, HAL_TIMEOUT or HAL_ERROR on failure.
 */
HAL_StatusTypeDef w25q_wait_busy(SPI_HandleTypeDef *hspi) {
    uint8_t cmd[2] = {CMD_READ_STATUS, 0xFF};
    uint8_t rx[2] = {0};
    uint32_t start = HAL_GetTick();

    do {
        HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hspi, cmd, rx, 2, SPI_TIMEOUT);
        if (status != HAL_OK) {
            LOG_INFO("Read Status failed: %d\r\n", status);
            return status;
        }
        if ((HAL_GetTick() - start) > 1000) {
            LOG_INFO("Wait Busy timeout\r\n");
            return HAL_TIMEOUT;
        }
    } while (rx[1] & 0x01);

    return HAL_OK;
}

/**
 * @brief Erase a sector (4 KB) at specified address.
 * @param hspi Pointer to SPI handle.
 * @param addr 24-bit sector address (must be sector-aligned).
 * @return HAL_OK on success, HAL_ERROR on failure.
 */
HAL_StatusTypeDef w25q_erase_sector(SPI_HandleTypeDef *hspi, uint32_t addr) {
    addr &= W25Q_MAX_ADDR;  // Mask to 24 bits
    uint8_t cmd[4] = {CMD_SECTOR_ERASE,
                      (uint8_t)(addr >> 16),
                      (uint8_t)(addr >> 8),
                      (uint8_t)addr};

    HAL_StatusTypeDef status = w25q_write_enable(hspi);
    if (status != HAL_OK) {
        return status;
    }
    status = HAL_SPI_Transmit(hspi, cmd, 4, SPI_TIMEOUT);
    if (status != HAL_OK) {
        LOG_INFO("Sector Erase failed at 0x%06lx: %d\r\n", addr, status);
        return status;
    }
    return w25q_wait_busy(hspi);
}

/**
 * @brief Write data to a page (256 bytes max) at specified address.
 * @param hspi Pointer to SPI handle.
 * @param addr 24-bit page start address (must be page-aligned).
 * @param data Pointer to data to write.
 * @param len Length of data (max 256 bytes).
 * @return HAL_OK on success, HAL_ERROR on failure.
 */
HAL_StatusTypeDef w25q_write_page(SPI_HandleTypeDef *hspi, uint32_t addr, uint8_t *data, uint16_t len) {
    if (data == NULL || len > 256) {
        LOG_INFO("Invalid write parameters: data=%p, len=%u\r\n", data, len);
        return HAL_ERROR;
    }
    addr &= W25Q_MAX_ADDR;  // Mask to 24 bits

    uint8_t cmd[4 + 256] = {0};
    cmd[0] = CMD_PAGE_PROGRAM;
    cmd[1] = (uint8_t)(addr >> 16);
    cmd[2] = (uint8_t)(addr >> 8);
    cmd[3] = (uint8_t)addr;
    memcpy(&cmd[4], data, len);

    HAL_StatusTypeDef status = w25q_write_enable(hspi);
    if (status != HAL_OK) {
        return status;
    }
    status = HAL_SPI_Transmit(hspi, cmd, 4 + len, SPI_TIMEOUT);
    if (status != HAL_OK) {
        LOG_INFO("Page Program failed at 0x%06lx: %d\r\n", addr, status);
        return status;
    }
    return w25q_wait_busy(hspi);
}

/**
 * @brief Read data from specified 24-bit address in W25Q128JV.
 * @param hspi Pointer to SPI handle.
 * @param addr 32-bit address (only lower 24 bits used, 0x000000 to 0xFFFFFF).
 * @param rx Pointer to buffer to store read data.
 * @param len Length of data to read (max 65535 bytes).
 * @return HAL_OK on success, HAL_ERROR on invalid parameters or SPI failure.
 */
HAL_StatusTypeDef w25q_read(SPI_HandleTypeDef *hspi, uint32_t addr, uint8_t *rx, uint16_t len) {
    if (rx == NULL || len == 0 || len > W25Q_MAX_READ_LEN || (addr + len - 1) > W25Q_MAX_ADDR) {
        LOG_INFO("Invalid read parameters: addr=0x%lx, rx=%p, len=%u\r\n", addr, rx, len);
        return HAL_ERROR;
    }

    addr &= W25Q_MAX_ADDR;

    // Allocate temporary buffer on heap
    uint8_t *rx_buf = (uint8_t *)malloc(4 + len);
    if (rx_buf == NULL) {
        LOG_INFO("Memory allocation failed for %u bytes\r\n", 4 + len);
        return HAL_ERROR;
    }
    memset(rx_buf, 0, 4 + len);

    // Prepare command
    rx_buf[0] = CMD_READ_DATA;
    rx_buf[1] = (uint8_t)(addr >> 16);
    rx_buf[2] = (uint8_t)(addr >> 8);
    rx_buf[3] = (uint8_t)addr;

    HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hspi, rx_buf, rx_buf, 4 + len, SPI_TIMEOUT);
    if (status != HAL_OK) {
        LOG_INFO("Read Data failed: %d\r\n", status);
        free(rx_buf);
        return status;
    }

    memcpy(rx, &rx_buf[4], len);
    free(rx_buf);
    return HAL_OK;
}


HAL_StatusTypeDef flash_erase_sector(SPI_Flash *self, uint32_t addr) {
    return w25q_erase_sector(self->hspi, addr);
}

HAL_StatusTypeDef flash_write_page(SPI_Flash *self, uint32_t addr, uint8_t *data, uint16_t len) {
    return w25q_write_page(self->hspi, addr, data, len);
}

HAL_StatusTypeDef flash_read(SPI_Flash *self, uint32_t addr, uint8_t *rx, uint16_t len) {
    return w25q_read(self->hspi, addr, rx, len);
}