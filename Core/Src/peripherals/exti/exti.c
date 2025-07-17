#include "main.h"
#include "mongoose.h"
#include "peripherals/timer/htim8.h"



void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // if (GPIO_Pin == USER_BUTTON_Pin) 
    // {
    //     // HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);

    //     Toggle_PWM(&htim8, TIM_CHANNEL_4);

    //     // Start/stop timer or toggle clock output here
    // }
    return;
}