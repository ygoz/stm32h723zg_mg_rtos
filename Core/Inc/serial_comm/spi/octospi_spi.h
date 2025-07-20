#pragma once


HAL_StatusTypeDef W25Q128_SPI_Init(OSPI_HandleTypeDef* hospi);

HAL_StatusTypeDef W25Q128_SPI_EraseSector(OSPI_HandleTypeDef* hospi, uint32_t EraseStartAddress, uint32_t EraseEndAddress);

HAL_StatusTypeDef W25Q128_SPI_Write(OSPI_HandleTypeDef* hospi, uint8_t* pData, uint32_t WriteAddr, uint32_t Size);

HAL_StatusTypeDef W25Q128_SPI_Read(OSPI_HandleTypeDef* hospi,uint8_t* pData, uint32_t ReadAddr, uint32_t Size);