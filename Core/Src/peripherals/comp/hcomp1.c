#include "peripherals/comp/hcomp1.h"




COMP_HandleTypeDef hcomp1;





void MX_COMP1_Init(void){

  /* USER CODE BEGIN COMP1_Init 0 */

  /* USER CODE END COMP1_Init 0 */

  /* USER CODE BEGIN COMP1_Init 1 */

  /* USER CODE END COMP1_Init 1 */
  hcomp1.Instance = COMP1;
  hcomp1.Init.InvertingInput = COMP_INPUT_MINUS_IO1;
  hcomp1.Init.NonInvertingInput = COMP_INPUT_PLUS_IO1;
  hcomp1.Init.OutputPol = COMP_OUTPUTPOL_NONINVERTED;
  hcomp1.Init.Hysteresis = COMP1_HYSTERESIS_MODE; // config hysteresis
  hcomp1.Init.BlankingSrce = COMP_BLANKINGSRC_NONE;
  hcomp1.Init.Mode = COMP_POWERMODE_HIGHSPEED; // only relevent for power, not important for hageneral
  hcomp1.Init.WindowMode = COMP_WINDOWMODE_DISABLE;
  hcomp1.Init.TriggerMode = COMP1_TRIGGER_MODE; // config this, calls interrupt
  if (HAL_COMP_Init(&hcomp1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN COMP1_Init 2 */

  /* USER CODE END COMP1_Init 2 */

}


void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp)
{
    if (hcomp->Instance == COMP1) {
        if (HAL_COMP_GetOutputLevel(hcomp) == COMP_OUTPUT_LEVEL_HIGH) {
            // INP > INM
            MG_INFO(("COMP1: Rising edge detected"));
        } else {
            // INP <= INM
            MG_INFO(("COMP1: Falling edge detected"));
        }
    }
}


HAL_StatusTypeDef comp1_get_value(uint32_t *comp1_output)
{
    HAL_StatusTypeDef status = HAL_OK;
    if (comp1_output == NULL) {
        MG_INFO(("Invalid pointer provided for COMP1 output"));
        return HAL_ERROR;
    }

    // Start the comparator
    #if COMP1_MODE == COMP1_MODE_POLLING
        status = HAL_COMP_Start(&hcomp1);

        *comp1_output = HAL_COMP_GetOutputLevel(&hcomp1);
        MG_INFO(("COMP1 output level: %s", *comp1_output == COMP_OUTPUT_LEVEL_HIGH ? "HIGH" : "LOW"));

        status = HAL_COMP_Stop(&hcomp1);
    #elif COMP1_MODE == COMP1_MODE_IT
        status = HAL_COMP_Start_IT(&hcomp1);

        *comp1_output = HAL_COMP_GetOutputLevel(&hcomp1);
        MG_INFO(("COMP1 output level: %s", *comp1_output == COMP_OUTPUT_LEVEL_HIGH ? "HIGH" : "LOW"));
    #elif COMP1_MODE == COMP1_MODE_OFF
        MG_INFO(("COMP1 is on COMP1_MODE_OFF"));
        return HAL_ERROR;
    #endif

    if (status != HAL_OK) {
        MG_INFO(("COMP1 start failed with status %d", status));
    }
    return status;
}