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



/**
 * @brief Retrieves the DMA buffer pointer associated with the given ADC handle.
 *
 * @param[in] hadc Pointer to the ADC handle (e.g., &hadc1, &hadc2, &hadc3).
 *
 * @return Pointer to the uint16_t DMA buffer for the given ADC handle.
 *         - If `hadc` is &hadc3, returns `adc3_dma_buffer`.
 * @example
 *   uint16_t *dma_buffer = adc_get_dma_buffer(&hadc3);
 *   if (dma_buffer) {
 *       printf("First DMA sample: %u\n", dma_buffer[0]);
 *   } else {
 *       printf("DMA buffer unavailable.\n");
 *   }
 */
uint16_t * adc_get_dma_buffer(ADC_HandleTypeDef *hadc);




/**
 * @brief Generates an HTTP-style response string based on the ADC status and mode.
 *
 * This function reads the ADC value (either by polling or DMA) from the specified ADC handle (`hadc`)
 * and fills the provided response buffer with a formatted string containing the ADC result or an error message.
 * It also returns an HTTP-like status code indicating the success or failure of the operation.
 *
 * @param[out] adc_value Pointer to a uint16_t where the polled ADC value will be stored (only used in polling mode).
 * @param[out] response Pointer to a character buffer where the formatted response message will be written.
 * @param[in] response_size Size of the `response` buffer in bytes. Ensure it is large enough to hold the output string.
 * @param[in] hadc Pointer to the ADC handle (e.g., &hadc1, &hadc2, &hadc3) used for reading the ADC value.
 * @param[in] ADC_POLLING_OR_DMA_MODE Mode of ADC operation:
 *             - `ADC_DMA_MODE` for reading values from DMA buffer.
 *             - `ADC_POLLING_MODE` for polling a single ADC value.
 * @param[in] ADC_HANDLE_STATUS Handle status indicating whether the ADC is ON or OFF:
 *             - `HANDLE_ON` to enable ADC read.
 *             - `HANDLE_OFF` to disable ADC read.
 *
 * @return uint16_t HTTP-like status code:
 *             - 200: Success, ADC value(s) read successfully.
 *             - 400: ADC handle is off or invalid mode/status.
 *             - 500: ADC read failed or DMA buffer error.
 *
 * @note - If `ADC_POLLING_OR_DMA_MODE` is `ADC_DMA_MODE`, this function calls `adc_get_dma_buffer(hadc)`
 *         to retrieve a pointer to the DMA buffer.
 *       - If `ADC_POLLING_OR_DMA_MODE` is `ADC_POLLING_MODE`, this function calls `adc_polling_get_value(hadc, adc_value, 100)`
 *         with a timeout of 100ms.
 *       - The function formats the result into `response` using snprintf, e.g., "adc value : %u, %u\n".
 *       - Ensure `response_size` is sufficient (suggested at least 64 bytes).
 *
 * @example
 *   uint16_t adc_value = 0;
 *   char response[256] = {0};
 *   uint16_t status = adc_get_http_response(&adc_value, response, sizeof(response),
 *                                           &hadc3, ADC_POLLING_MODE, HANDLE_ON);
 *   printf("HTTP Status: %u\n", status);
 *   printf("Response: %s\n", response);
 */
uint16_t adc_get_http_response(
    uint16_t *adc_value,
    char *response,
    size_t response_size,
    ADC_HandleTypeDef *hadc,
    uint8_t ADC_POLLING_OR_DMA_MODE,
    uint8_t ADC_HANDLE_STATUS
);