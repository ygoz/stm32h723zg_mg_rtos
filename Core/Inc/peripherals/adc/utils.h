/**
 * @file utils.h
 * @author Yam Goz
 * @brief Utility header for ADC code management.
 * 
 * This header file provides utility definitions and function prototypes 
 * for managing and accessing ADC peripherals (ADC1, ADC2, ADC3) on the STM32H7 series. 
 * It includes data structures, initialization, polling methods, and attribute macros 
 * to support memory alignment and BDMA (Basic DMA) transfers.
 * 
 * The functions and constants defined here are intended to be used by all ADC handles, 
 * providing a common interface for ADC initialization and value retrieval.
 * 
 * @version 0.1
 * @date 2025-05-21
 * 
 * @copyright Copyright (c) 2025
 * 
 * @note This file should be included wherever ADC utilities are required.
 */

#pragma once


#include "stm32h7xx_hal.h"
#include <stdint.h>


// Indicates whether the ADC handle is enabled or disabled
#define HANDLE_OFF 0
#define HANDLE_ON  1

// Defines the ADC operating mode: polling mode or DMA mode
#define ADC_POLLING_MODE 2
#define ADC_DMA_MODE     3


/**
 * @brief Attribute to place data in the SRAM4 memory region for BDMA access.
 */
#define SRAM4_BDMA __attribute__((section(".sram4")))


/**
 * @brief Attribute to ensure data is aligned to a 4-byte boundary, required for DMA operations.
 *
 * Many DMA controllers (including the STM32 DMA/BDMA) require data to be aligned to a certain boundary (in this case, 4 bytes)
 * for efficient and correct memory transfers. This is because the DMA hardware reads memory in word (32-bit) units and expects 
 * addresses to be aligned accordingly. If the data is not properly aligned, the transfer may result in errors, inefficient 
 * performance, or even hard faults.
 *
 * @note This is especially important when using DMA to transfer data types smaller than 4 bytes (e.g., uint16_t), since 
 * misalignment can cause partial or incorrect transfers.
 */
#define ALIGN4 __attribute__((aligned(4)))



typedef enum {
    ADC_STATUS_NONE      = 0,  ///< No ADCs initialized successfully
    ADC_STATUS_ADC1      = 1,  ///< Only ADC1 initialized
    ADC_STATUS_ADC2      = 2,  ///< Only ADC2 initialized
    ADC_STATUS_ADC3      = 4,  ///< Only ADC3 initialized
    ADC_STATUS_ADC1_ADC2 = 3,  ///< ADC1 and ADC2 initialized
    ADC_STATUS_ADC1_ADC3 = 5,  ///< ADC1 and ADC3 initialized
    ADC_STATUS_ADC2_ADC3 = 6,  ///< ADC2 and ADC3 initialized
    ADC_STATUS_ALL       = 7   ///< All ADCs initialized successfully
} ADC_StatusBitmap;



/**
 * @brief Initialize all configured ADC peripherals and start DMA if applicable.
 *
 * This function initializes ADC1, ADC2, and ADC3 based on the compile-time definitions:
 * - `ADC1_HANDLE_STATUS`: Whether ADC1 is enabled (`HANDLE_ON` or `HANDLE_OFF`).
 * - `ADC2_HANDLE_STATUS`: Whether ADC2 is enabled.
 * - `ADC3_HANDLE_STATUS`: Whether ADC3 is enabled.
 * 
 * For each enabled ADC, it calls `init_adc()` with the appropriate ADC handle and configuration. 
 * If ADC3 is configured to use DMA (`ADC3_POLLING_OR_DMA_MODE == ADC_DMA_MODE`), it starts the ADC3 DMA 
 * transfer using `HAL_ADC_Start_DMA()`.
 *
 * @return A bitmap representing the initialization status of ADCs, where each bit corresponds to a specific ADC:
 * - `ADC_STATUS_ADC1` (1): ADC1 initialized.
 * - `ADC_STATUS_ADC2` (2): ADC2 initialized.
 * - `ADC_STATUS_ADC3` (4): ADC3 initialized.
 * The bitmap can be used to check which ADCs were initialized successfully.
 *
 * @note The function also logs the initialization status using `MG_INFO`.
 */
uint8_t adc_init_all_handles(void);



/**
 * @brief Perform a single ADC conversion using polling and retrieve the result.
 *
 * This function starts an ADC conversion on the provided ADC handle and polls for its completion
 * within the specified timeout period. If the conversion completes successfully, it retrieves the
 * ADC conversion value and stores it in the location pointed to by @p adc_value.
 *
 * @param hadc Pointer to the ADC handle (`ADC_HandleTypeDef`) to use for the conversion.
 * @param adc_value Pointer to a variable where the ADC conversion result will be stored.
 * @param polling_timeout Timeout (in milliseconds) for the polling operation.
 *
 * @return HAL status:
 * - `HAL_OK` if the conversion completed successfully.
 * - `HAL_ERROR`, `HAL_BUSY`, or `HAL_TIMEOUT` if the conversion start or polling failed.
 *
 * @note This function logs errors using `MG_INFO` if starting or polling the ADC fails.
 * @note The result is truncated to 16 bits, even if the ADC has a higher resolution.
 */
HAL_StatusTypeDef adc_polling_get_value(ADC_HandleTypeDef *hadc, uint16_t *adc_value, uint16_t polling_timeout);