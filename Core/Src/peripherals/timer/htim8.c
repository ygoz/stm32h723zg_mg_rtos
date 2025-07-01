#include "peripherals/timer/htim8.h"


TIM_HandleTypeDef htim8;



/**
  * @brief TIM8 Initialization Function
  * @param None
  * @retval None
  */
void MX_TIM8_Init(void){

  /* USER CODE BEGIN TIM8_Init 0 */

  /* USER CODE END TIM8_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM8_Init 1 */

  /* USER CODE END TIM8_Init 1 */
  htim8.Instance = TIM8;
  htim8.Init.Prescaler = TIM8_PRESCALER; // config 
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = TIM8_PERIOD; // config
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = TIM8_CH4_PULSE; // config
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_Init 2 */

  /* USER CODE END TIM8_Init 2 */
  HAL_TIM_MspPostInit(&htim8);

}




static HAL_StatusTypeDef tim_status;
static uint8_t pwm_running = 0;  // 0 = stopped, 1 = running

void Toggle_PWM(TIM_HandleTypeDef *htim, uint32_t channel) {
#if TIM8_MODE == TIM_MODE_PWM
    // Check the current state of the timer
    HAL_TIM_StateTypeDef tim_state = HAL_TIM_PWM_GetState(htim);

    MG_INFO(("Timer state: %d", tim_state));

    if (tim_state == HAL_TIM_STATE_BUSY) {
        // Timer is busy: stop PWM and reset our running flag
        tim_status = HAL_TIM_PWM_Stop(htim, channel);
        if (tim_status == HAL_OK) {
            pwm_running = 0;
            MG_INFO(("PWM stopped because timer is busy."));
        } else {
            MG_INFO(("Failed to stop PWM when timer busy, status: %d", tim_status));
        }
        return;
    }

    if (!pwm_running) {
        // PWM is currently stopped → start it
        tim_status = HAL_TIM_PWM_Start(htim, channel);
        if (tim_status == HAL_OK) {
            pwm_running = 1;
            MG_INFO(("PWM started on Timer Channel (e.g., PC9 for TIM8 CH4)"));
        } else {
            MG_INFO(("Failed to start PWM, status: %d", tim_status));
        }
    } else {
        // PWM is currently running → stop it
        tim_status = HAL_TIM_PWM_Stop(htim, channel);
        if (tim_status == HAL_OK) {
            pwm_running = 0;
            MG_INFO(("PWM stopped on Timer Channel (e.g., PC9 for TIM8 CH4)"));
        } else {
            MG_INFO(("Failed to stop PWM, status: %d", tim_status));
        }
    }
#else
    MG_INFO(("TIM8 is disabled, mode TIM_MODE_DISABLED));
#endif
}