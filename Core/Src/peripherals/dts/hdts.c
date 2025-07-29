/**
 * @file hdts.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-06-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#include "peripherals/dts/hdts.h"

DTS_HandleTypeDef hdts;



/**
  * @brief DTS Initialization Function
  * @param None
  * @retval None
  */
void MX_DTS_Init(void){

  /* USER CODE BEGIN DTS_Init 0 */

  /* USER CODE END DTS_Init 0 */

  /* USER CODE BEGIN DTS_Init 1 */

  /* USER CODE END DTS_Init 1 */
  hdts.Instance = DTS;
  hdts.Init.QuickMeasure = DTS_QUICKMEAS_DISABLE;
  hdts.Init.RefClock = DTS_REFCLKSEL_PCLK;
  hdts.Init.TriggerInput = DTS_TRIGGER_HW_NONE;
  hdts.Init.SamplingTime = DTS_SMP_TIME_15_CYCLE;
  hdts.Init.Divider = 100;
  hdts.Init.HighThreshold = 0x0;
  hdts.Init.LowThreshold = 0x0;
  if (HAL_DTS_Init(&hdts) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DTS_Init 2 */

  /* USER CODE END DTS_Init 2 */

}


HAL_StatusTypeDef dts_get_temperature(int32_t *temperature)
{
    if (temperature == NULL) {
        MG_INFO(("DTS temperature pointer is NULL"));
        return HAL_ERROR;
    }

    // Start the DTS sensor
    HAL_StatusTypeDef status = HAL_DTS_Start(&hdts);
    if (status != HAL_OK) {
        MG_INFO(("DTS start failed with status %d", status));
        return status;
    }

    // Get the temperature reading
    status = HAL_DTS_GetTemperature(&hdts, temperature);
    if (status != HAL_OK) {
        MG_INFO(("DTS get temperature failed with status %d", status));
    }

    status = HAL_DTS_Stop(&hdts);
    if (status != HAL_OK) {
        MG_INFO(("DTS stop failed with status %d", status));
    }

    return status;
}