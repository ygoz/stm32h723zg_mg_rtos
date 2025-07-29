/**
 * @file htim8.h
 * @author Yam Goz
 * @brief Configuration and control interface for TIM8 PWM generation on STM32H7.
 * @version 0.1
 * @date 2025-07-01
 * 
 * @copyright Copyright (c) 2025
 * 
 * @details
 * This header provides macros, function prototypes, and documentation for configuring
 * and controlling TIM8 as a PWM output on STM32H7 microcontrollers.
 * 
 * Features:
 * - Definitions of prescaler, period, and pulse width to generate a precise PWM signal.
 * - Utility functions to start, stop, and toggle the PWM output on TIM8 channels.
 * - Documentation on how to calculate PWM frequency and duty cycle based on timer configuration.
 * 
 * TIM8 is typically clocked from the APB2 peripheral bus (e.g., 275 MHz AHB4 clock on STM32H7)
 * and is capable of generating high-resolution PWM signals suitable for motor control,
 * LED dimming, and general-purpose signal generation.
 * 
 * Usage:
 * - Configure the macros TIM8_PRESCALER, TIM8_PERIOD, and TIM8_CH4_PULSE to set your
 *   desired PWM frequency and duty cycle.
 * - Use the provided functions to initialize or toggle the PWM signal.
 * 
 * Example formula to calculate PWM frequency:
 * 
 *      f_pwm = TIM_CLK / ((Prescaler + 1) * (Period + 1))
 * 
 * Where:
 *   - TIM_CLK is the timer input clock frequency (e.g., 275 MHz).
 *   - Prescaler divides TIM_CLK to determine timer tick frequency.
 *   - Period sets the PWM cycle length in ticks.
 * 
 * Duty cycle calculation:
 * 
 *      DutyCycle = (Pulse / (Period + 1)) * 100 [%]
 * 
 */

#include "main.h"
#include "mongoose.h"
#include "HaGeneral/HaGeneral_config.h"


#define TIM_MODE_DISABLED 1
#define TIM_MODE_PWM 2



// ============================
// TIM8 Configuration Checks
// ============================

// ---------- Prescaler ----------
#ifndef TIM8_PRESCALER
#error "TIM8_PRESCALER is not defined. Please define it in HaGeneral_config.h (e.g., #define TIM8_PRESCALER 0)"
#endif

#if (TIM8_PRESCALER > 65535)
#error "Invalid TIM8_PRESCALER value. Allowed range: 0 to 65535."
#endif

// ---------- Period ----------
#ifndef TIM8_PERIOD
#error "TIM8_PERIOD is not defined. Please define it in HaGeneral_config.h (e.g., #define TIM8_PERIOD 1374)"
#endif

#if (TIM8_PERIOD > 65535)
#error "Invalid TIM8_PERIOD value. Allowed range: 0 to 65535."
#endif

// ---------- CH4 Pulse ----------
#ifndef TIM8_CH4_PULSE
#error "TIM8_CH4_PULSE is not defined. Please define it in HaGeneral_config.h (e.g., #define TIM8_CH4_PULSE 687)"
#endif

#if (TIM8_CH4_PULSE > 65535)
#error "Invalid TIM8_CH4_PULSE value. Allowed range: 0 to 65535."
#endif

#if (TIM8_CH4_PULSE > TIM8_PERIOD)
#error "Invalid TIM8_CH4_PULSE: must be less than or equal to TIM8_PERIOD."
#endif

// ---------- Mode ----------
#ifndef TIM8_MODE
#error "TIM8_MODE is not defined. Please define it in HaGeneral_config.h (e.g., #define TIM8_MODE TIM_MODE_PWM)"
#endif

#if (TIM8_MODE != TIM_MODE_PWM) && (TIM8_MODE != TIM_MODE_DISABLED)
#error "Invalid TIM8_MODE value. Allowed values: TIM_MODE_PWM or TIM_MODE_DISABLED."
#endif



extern TIM_HandleTypeDef htim8;


void MX_TIM8_Init(void);


/**
 * @brief Toggles the PWM output on a specified timer channel.
 * 
 * This function starts the PWM signal on the specified timer channel if it is currently stopped,
 * or stops it if it is currently running. It also handles the case when the timer is busy by
 * stopping the PWM to avoid conflicts.
 * 
 * The function internally tracks the PWM running state using a static flag, since the HAL does not
 * provide a direct method to check if PWM is active on a channel.
 * 
 * @param[in] htim    Pointer to the TIM handle structure (e.g., &htim8).
 * @param[in] channel Timer channel to toggle PWM on (e.g., TIM_CHANNEL_4).
 * 
 * @note This function logs status messages using MG_INFO().
 * 
 * @warning Ensure the timer is properly initialized for PWM mode before calling this function.
 */
void Toggle_PWM(TIM_HandleTypeDef *htim, uint32_t channel);