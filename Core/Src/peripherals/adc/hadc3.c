/**
 * @file hadc3.c
 * @author Yam Goz
 * @brief STM32H723ZGT6 ADC3 Peripheral Configuration and Handling
 * @version 0.1
 * @date 2025-05-28
 * 
 * @details
 * This file provides the configuration and handling for ADC3 on the STM32H723ZGT6.
 * It supports both polling and DMA modes of operation, as well as the analog watchdog feature.
 * The DMA buffer is placed in SRAM4 to enable BDMA (Basic DMA) access, which is limited to SRAM4.
 * The analog watchdog functionality allows detection of threshold anomalies, with customizable
 * filtering and thresholds defined in preprocessor macros.
 * 
 * Key Features:
 * - Initialization of ADC3 peripheral for polling or DMA mode.
 * - DMA buffer configured in SRAM4 (16KB) for BDMA usage.
 * - Analog watchdog configuration for anomaly detection, with interrupt support.
 * - Modular functions to fetch ADC3 data in both DMA and polling modes.
 * - Configurable ADC3 sampling time, resolution, oversampling, and data alignment.
 * - Custom callback (adc3_wdg_process_anomaly) for handling analog watchdog anomalies.
 * - DMA interrupt initialization for BDMA Channel 0.
 * 
 * @note
 * Ensure that the linker script places the DMA buffer (adc3_dma_buffer) in SRAM4,
 * as BDMA is only connected to this region.
 * 
 * @warning
 * - Ensure preprocessor macros (like ADC3_ANALOG_WATCHDOG_HIGH_THRESHOLD) are defined within valid ranges.
 * - DMA configuration assumes BDMA is used and that its clock is enabled.
 * - This file is specifically for ADC3. For ADC1/ADC2, refer to their respective files.
 * 
 * @see MX_ADC3_Init() for peripheral initialization.
 * @see adc3_get_value() for DMA mode data retrieval.
 * @see adc3_get_value() for polling mode data retrieval.
 * @see adc3_wdg_process_anomaly() for handling analog watchdog events.
 * @see MX_BDMA_Init() for BDMA controller initialization.
 */

#include "peripherals/adc/hadc3.h"
#include "peripherals/adc/utils.h"
#include "cmsis_os.h"  // for osDelay()
#include "mongoose.h"



ADC_HandleTypeDef hadc3;

DMA_HandleTypeDef hdma_adc3;




#if ADC3_POLLING_OR_DMA_MODE == ADC_DMA_MODE 

// define buffer for adc dma data
SRAM4_BDMA uint16_t adc3_dma_buffer[ADC3_DMA_BUFFER_SIZE] ALIGN4;

// return the dma buffer pointer
uint16_t* adc3_get_value(void) {
 return adc3_dma_buffer;  // Returns a pointer to the first element
}

#elif ADC3_POLLING_OR_DMA_MODE == ADC_POLLING_MODE

// set adc value and return HAL Status
HAL_StatusTypeDef adc3_get_value(uint16_t *adc_value) {
  return adc_polling_get_value(&hadc3, adc_value, ADC3_POLLING_TIMEOUT);
}
#endif


// this function will be called if the watchdog passes a threshold set
void adc3_wdg_process_anomaly(void){
  MG_INFO(("ADC3 ANOMALLY!!"));
}


uint16_t adc3_get_http_response(uint16_t *adc_value, char response[256]) {

  uint16_t http_status_code = {0};

  #if ADC3_HANDLE_STATUS == HANDLE_OFF
      snprintf(response, 256, "adc3 handle is off");
      http_status_code = 400;
      return http_status_code;
  
  #elif ADC3_HANDLE_STATUS == HANDLE_ON
      #if ADC3_POLLING_OR_DMA_MODE == ADC_DMA_MODE
          uint16_t* my_buffer = adc3_get_value();
          snprintf(response, 256, "adc3 value : %u, %u\n", my_buffer[0], my_buffer[1]);
          http_status_code = 200;
          return http_status_code;
  
      #elif ADC3_POLLING_OR_DMA_MODE == ADC_POLLING_MODE
          if (adc3_get_value(adc_value) == HAL_OK) {
              snprintf(response, 256, "adc3 value : %u\n", *adc_value);
              http_status_code = 200;
              return http_status_code;
          } else {
              snprintf(response, 256, "adc3 read failed\n");
              http_status_code = 500;
              return http_status_code;
            }
      #endif
  #endif
  }

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
 void MX_ADC3_Init(void){
 
   /* USER CODE BEGIN ADC3_Init 0 */
 
   /* USER CODE END ADC3_Init 0 */
 
   ADC_ChannelConfTypeDef sConfig = {0};
 
   /* USER CODE BEGIN ADC3_Init 1 */
 
   /* USER CODE END ADC3_Init 1 */
 
   /** Common config
   */
   hadc3.Instance = ADC3;
   hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
   hadc3.Init.Resolution = ADC_RESOLUTION_12B;
   hadc3.Init.DataAlign = ADC3_DATAALIGN_RIGHT;
   hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
   hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
   hadc3.Init.LowPowerAutoWait = DISABLE;

   hadc3.Init.ContinuousConvMode = ENABLE;
   #if ADC3_ANALOG_WATCHDOG == HANDLE_OFF
   hadc3.Init.ContinuousConvMode = DISABLE;
   #endif

   #if ADC3_POLLING_OR_DMA_MODE == ADC_POLLING_MODE
    hadc3.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    hadc3.Init.DMAContinuousRequests = DISABLE;
   #elif ADC3_POLLING_OR_DMA_MODE == ADC_DMA_MODE
    hadc3.Init.ContinuousConvMode = ENABLE;
    hadc3.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
    hadc3.Init.DMAContinuousRequests = ENABLE;
   #endif
   hadc3.Init.NbrOfConversion = 1;
   hadc3.Init.DiscontinuousConvMode = DISABLE;
   hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
   hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
   hadc3.Init.SamplingMode = ADC_SAMPLING_MODE_NORMAL;
   hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
   hadc3.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
   hadc3.Init.OversamplingMode = DISABLE;
   hadc3.Init.Oversampling.Ratio = ADC3_OVERSAMPLING_RATIO_2;
   if (HAL_ADC_Init(&hadc3) != HAL_OK)
   {
     Error_Handler();
   }

   #if ADC3_ANALOG_WATCHDOG == HANDLE_ON
    /** Configure Analog WatchDog 1
     */
    ADC_AnalogWDGConfTypeDef AnalogWDGConfig = {0};
    AnalogWDGConfig.WatchdogNumber = ADC_ANALOGWATCHDOG_1;
    AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG;
    AnalogWDGConfig.Channel = ADC_CHANNEL_1;
    AnalogWDGConfig.ITMode = ENABLE;
    AnalogWDGConfig.HighThreshold = ADC3_ANALOG_WATCHDOG_HIGH_THRESHOLD;
    AnalogWDGConfig.LowThreshold = ADC3_ANALOG_WATCHDOG_LOW_THRESHOLD;
    AnalogWDGConfig.FilteringConfig = ADC3_AWD_FILTERING_4SAMPLES;
    if (HAL_ADC_AnalogWDGConfig(&hadc3, &AnalogWDGConfig) != HAL_OK)
    {
      Error_Handler();
    }
   #endif
 
   /** Configure Regular Channel
   */
   sConfig.Channel = ADC_CHANNEL_1;
   sConfig.Rank = ADC_REGULAR_RANK_1;
   sConfig.SamplingTime = ADC3_SAMPLETIME_2CYCLES_5;
   sConfig.SingleDiff = ADC3_SINGLE_OR_DOUBLE_ENDED;
   sConfig.OffsetNumber = ADC_OFFSET_NONE;
   sConfig.Offset = 0;
   sConfig.OffsetSign = ADC3_OFFSET_SIGN_NEGATIVE;
   if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
   {
     Error_Handler();
   }
   /* USER CODE BEGIN ADC3_Init 2 */
 
   /* USER CODE END ADC3_Init 2 */
 
 }


/**
  * Enable BDMA controller clock
  */
void MX_BDMA_Init(void){
 
   /* DMA controller clock enable */
   __HAL_RCC_BDMA_CLK_ENABLE();
 
   /* DMA interrupt init */
   /* BDMA_Channel0_IRQn interrupt configuration */
   HAL_NVIC_SetPriority(BDMA_Channel0_IRQn, 5, 0);
   HAL_NVIC_EnableIRQ(BDMA_Channel0_IRQn);
 
 }