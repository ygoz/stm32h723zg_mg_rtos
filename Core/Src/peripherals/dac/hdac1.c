#include "peripherals/dac/hdac1.h"



DAC_HandleTypeDef hdac1;


void MX_DAC1_Init(void){

  /* USER CODE BEGIN DAC1_Init 0 */

  /* USER CODE END DAC1_Init 0 */

  DAC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN DAC1_Init 1 */

  /* USER CODE END DAC1_Init 1 */

  /** DAC Initialization
  */
  hdac1.Instance = DAC1;
  if (HAL_DAC_Init(&hdac1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT1 config
  */
  sConfig.DAC_SampleAndHold = DAC_SAMPLEANDHOLD_DISABLE;
  sConfig.DAC_Trigger = DAC_TRIGGER_NONE;
  sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
  sConfig.DAC_ConnectOnChipPeripheral = DAC_CHIPCONNECT_DISABLE;
  sConfig.DAC_UserTrimming = DAC_TRIMMING_FACTORY;
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** DAC channel OUT2 config
  */
  if (HAL_DAC_ConfigChannel(&hdac1, &sConfig, DAC_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DAC1_Init 2 */

  /* USER CODE END DAC1_Init 2 */

}




HAL_StatusTypeDef dac1_set_value(uint32_t dac_value, uint32_t channel)
{
    if (
        dac_value == NULL ||
        (channel != DAC_CHANNEL_1 && channel != DAC_CHANNEL_2) ||
        (dac_value > 4095)
    ) {
        MG_INFO(("DAC1 input values are incorrect"));
        return HAL_ERROR;  // Invalid pointer, channel, or value
    }

    // Write the DAC value to channel 1 (12-bit right-aligned)
    HAL_StatusTypeDef status = HAL_DAC_Start(&hdac1, channel);
    if (status != HAL_OK) {
        // Failed to start ADC conversion
        MG_INFO(("ADC start failed with status %d", status));
        return status;
    }
    status = HAL_DAC_SetValue(&hdac1, channel, DAC_ALIGN_12B_R, dac_value);

    return status;
}