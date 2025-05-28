#include "peripherals/adc/hadc2.h"
#include "peripherals/adc/utils.h"
#include "cmsis_os.h"  // for osDelay()
#include "mongoose.h"
 
 
 
ADC_HandleTypeDef hadc2;




#if ADC2_POLLING_OR_DMA_MODE == ADC_DMA_MODE 

// define buffer for adc dma data

// return the dma buffer pointer
uint16_t* adc2_get_value(void) {
return adc2_dma_buffer;  // Returns a pointer to the first element
}

#elif ADC2_POLLING_OR_DMA_MODE == ADC_POLLING_MODE

// set adc value and return HAL Status
HAL_StatusTypeDef adc2_get_value(uint16_t *adc_value) {
return adc_polling_get_value(&hadc2, adc_value, ADC2_POLLING_TIMEOUT);
}
#endif


#if ADC2_ANALOG_WATCHDOG == HANDLE_ON
// this function will be called if the watchdog passes a threshold set
void adc2_wdg_process_anomaly(void){
MG_INFO(("ADC2 ANOMALLY!!"));
}
#endif

/**
 * @brief ADC2 Initialization Function
 * @param None
 * @retval None
 */

/**
  * @brief ADC2 Initialization Function
  * @param None
  * @retval None
  */
void MX_ADC2_Init(void){
 
   /* USER CODE BEGIN ADC2_Init 0 */
 
   /* USER CODE END ADC2_Init 0 */
 
   ADC_ChannelConfTypeDef sConfig = {0};
 
   /* USER CODE BEGIN ADC2_Init 1 */
 
   /* USER CODE END ADC2_Init 1 */
 
   /** Common config
   */
   hadc2.Instance = ADC2;
   hadc2.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
   hadc2.Init.Resolution = ADC_RESOLUTION_16B;
   hadc2.Init.DataAlign = ADC3_DATAALIGN_RIGHT;
   hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
   hadc2.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
   hadc2.Init.LowPowerAutoWait = DISABLE;
#if ADC2_POLLING_OR_DMA_MODE == ADC_POLLING_MODE
    hadc2.Init.ContinuousConvMode = DISABLE;
    hadc2.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
    hadc2.Init.DMAContinuousRequests = DISABLE;
#elif ADC2_POLLING_OR_DMA_MODE == ADC_DMA_MODE
    hadc2.Init.ContinuousConvMode = ENABLE;
    hadc2.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
    hadc2.Init.DMAContinuousRequests = ENABLE;
#endif
   hadc2.Init.NbrOfConversion = 1;
   hadc2.Init.DiscontinuousConvMode = DISABLE;
   hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
   hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
   hadc2.Init.SamplingMode = ADC_SAMPLING_MODE_NORMAL;
   hadc2.Init.Overrun = ADC_OVR_DATA_PRESERVED;
   hadc2.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
   hadc2.Init.OversamplingMode = DISABLE;
   hadc2.Init.Oversampling.Ratio = 2;
   if (HAL_ADC_Init(&hadc2) != HAL_OK)
   {
     Error_Handler();
   }

#if ADC2_ANALOG_WATCHDOG == HANDLE_ON
    /** Configure Analog WatchDog 1
     */
    ADC_AnalogWDGConfTypeDef AnalogWDGConfig = {0};
    AnalogWDGConfig.WatchdogNumber = ADC_ANALOGWATCHDOG_1;
    AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG;
    AnalogWDGConfig.Channel = ADC_CHANNEL_1;
    AnalogWDGConfig.ITMode = ENABLE;
    AnalogWDGConfig.HighThreshold = ADC2_ANALOG_WATCHDOG_HIGH_THRESHOLD;
    AnalogWDGConfig.LowThreshold = ADC2_ANALOG_WATCHDOG_LOW_THRESHOLD;
    AnalogWDGConfig.FilteringConfig = ADC2_AWD_FILTERING_4SAMPLES;
    if (HAL_ADC_AnalogWDGConfig(&hadc2, &AnalogWDGConfig) != HAL_OK)
    {
    Error_Handler();
    }
#endif

   /** Configure Regular Channel
   */
   sConfig.Channel = ADC_CHANNEL_2;
   sConfig.Rank = ADC_REGULAR_RANK_1;
   sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
   sConfig.SingleDiff = ADC2_SINGLE_OR_DOUBLE_ENDED;
   sConfig.OffsetNumber = ADC_OFFSET_NONE;
   sConfig.Offset = 0;
   sConfig.OffsetSignedSaturation = DISABLE;
   if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
   {
     Error_Handler();
   }
   /* USER CODE BEGIN ADC2_Init 2 */
 
   /* USER CODE END ADC2_Init 2 */
 }