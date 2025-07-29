#include "peripherals/adc/hadc1.h"
#include "peripherals/adc/utils.h"
#include "cmsis_os.h"  // for osDelay()
#include "mongoose.h"
 
 
 
ADC_HandleTypeDef hadc1;






#if ADC1_POLLING_OR_DMA_MODE == ADC_POLLING_MODE

// set adc value and return HAL Status
HAL_StatusTypeDef adc1_get_value(uint16_t *adc_value) {
return adc_polling_get_value(&hadc1, adc_value, ADC1_POLLING_TIMEOUT);
}
#endif


// this function will be called if the watchdog passes a threshold set
void adc1_wdg_process_anomaly(void){
MG_INFO(("ADC1 ANOMALLY!!"));
}


/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
void MX_ADC1_Init(void){
 
   /* USER CODE BEGIN ADC1_Init 0 */
 
   /* USER CODE END ADC1_Init 0 */
 
   ADC_ChannelConfTypeDef sConfig = {0};
   ADC_MultiModeTypeDef multimode = {0};
 
   /* USER CODE BEGIN ADC1_Init 1 */
 
   /* USER CODE END ADC1_Init 1 */
 
   /** Common config
   */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_16B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;


  hadc1.Init.ContinuousConvMode = ENABLE;
  #if ADC1_ANALOG_WATCHDOG == HANDLE_OFF
  hadc1.Init.ContinuousConvMode = DISABLE;
  #endif

#if ADC1_POLLING_OR_DMA_MODE == ADC_POLLING_MODE
    hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
#elif ADC1_POLLING_OR_DMA_MODE == ADC_DMA_MODE
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
    hadc1.Init.DMAContinuousRequests = ENABLE;
#endif
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
  hadc1.Init.OversamplingMode = DISABLE;
  hadc1.Init.Oversampling.Ratio = 1;
   if (HAL_ADC_Init(&hadc1) != HAL_OK)
   {
     Error_Handler();
   }

multimode.Mode = ADC_MODE_INDEPENDENT;
   if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
   {
     Error_Handler();
   }

#if ADC1_ANALOG_WATCHDOG == HANDLE_ON
    /** Configure Analog WatchDog 1
     */
    ADC_AnalogWDGConfTypeDef AnalogWDGConfig = {0};
    AnalogWDGConfig.WatchdogNumber = ADC_ANALOGWATCHDOG_1;
    AnalogWDGConfig.WatchdogMode = ADC_ANALOGWATCHDOG_SINGLE_REG;
    AnalogWDGConfig.Channel = ADC_CHANNEL_2;
    AnalogWDGConfig.ITMode = ENABLE;
    AnalogWDGConfig.HighThreshold = ADC1_ANALOG_WATCHDOG_HIGH_THRESHOLD;
    AnalogWDGConfig.LowThreshold = ADC1_ANALOG_WATCHDOG_LOW_THRESHOLD;
    if (HAL_ADC_AnalogWDGConfig(&hadc1, &AnalogWDGConfig) != HAL_OK)
    {
      Error_Handler();
    }
#endif

   /** Configure Regular Channel
   */
   sConfig.Channel = ADC_CHANNEL_2;
   sConfig.Rank = ADC_REGULAR_RANK_1;
   sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
   sConfig.SingleDiff = ADC1_SINGLE_OR_DOUBLE_ENDED;
   sConfig.OffsetNumber = ADC_OFFSET_NONE;
   sConfig.Offset = 0;
   sConfig.OffsetSignedSaturation = DISABLE;
   if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
   {
     Error_Handler();
   }
   /* USER CODE BEGIN ADC1_Init 2 */
 
   /* USER CODE END ADC1_Init 2 */
 }