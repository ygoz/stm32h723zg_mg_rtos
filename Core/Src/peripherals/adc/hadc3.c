/**
 * @file hadc3.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "peripherals/adc/hadc3.h"
#include "peripherals/adc/utils.h"
#include "cmsis_os.h"  // for osDelay()



ADC_HandleTypeDef hadc3;




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
   #if ADC3_POLLING_OR_DMA_MODE == ADC_POLLING_MODE
    hadc3.Init.ContinuousConvMode = DISABLE;
    hadc3.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DR;
   #elif ADC3_POLLING_OR_DMA_MODE == ADC_DMA_MODE
   hadc3.Init.ContinuousConvMode = ENABLE;
   hadc3.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
   #endif
   hadc3.Init.NbrOfConversion = 1;
   hadc3.Init.DiscontinuousConvMode = DISABLE;
   hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
   hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
   hadc3.Init.DMAContinuousRequests = DISABLE;
   hadc3.Init.SamplingMode = ADC_SAMPLING_MODE_NORMAL;
   hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
   hadc3.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
   hadc3.Init.OversamplingMode = DISABLE;
   hadc3.Init.Oversampling.Ratio = ADC3_OVERSAMPLING_RATIO_2;
   if (HAL_ADC_Init(&hadc3) != HAL_OK)
   {
     Error_Handler();
   }
 
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


// ADC POLL VS ADC DMA ------ @TODO: ADD DEFINES
uint16_t adc3_get_value(void) {

  #if ADC3_POLLING_OR_DMA_MODE == ADC_POLLING_MODE
    return adc_get_value(&hadc3);
  #endif

}