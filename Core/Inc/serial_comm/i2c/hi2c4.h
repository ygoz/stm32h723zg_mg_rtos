/**
 ******************************************************************************
 * @file    i2c4.h
 * @brief   I2C4 initialization header
 *
 * Provides conditional initialization declarations for I2C4 based on the
 * selected I2C mode (Standard or Fast Mode).
 *
 * 🧩 Pin Mapping:
 *   SDA: PF15 (AF4)
 *   SCL: PD12 (AF4)
 *
 * ⚙️ Modes:
 *   - Define `I2C_STANDARDMODE` to use Standard Mode (100 kHz)
 *     → Declares: void MX_I2C4_Init_StandardMode(void);
 *
 *   - Define `I2C_FASTMODE` to use Fast Mode (400 kHz)
 *     → Declares: void MX_I2C4_Init_FastMode(void);
 *
 * 💡 Usage:
 *   #define I2C_FASTMODE
 *   #include "i2c4.h"
 *   MX_I2C4_Init_FastMode();
 *
 ******************************************************************************
 */
#pragma once

#include "stm32h7xx_hal.h" // needed for I2C_HandleTypeDef 
#include "main.h" // needed for error_handling

//add user defines............
#define I2C4_MODE_STD 0
#define I2C4_MODE_FAST 1
#define I2C4_MODE_FAST_PLUS 2

#define I2C4_MODE I2C_MODE_FAST


#if I2C4_MODE == I2C4_MODE_STD
#define I2C4_GPIO_SPEED GPIO_SPEED_FREQ_LOW
#elif I2C4_MODE == I2C4_MODE_FAST
#define I2C4_GPIO_SPEED GPIO_SPEED_FREQ_MEDIUM
#elif I2C4_MODE == I2C4_MODE_FAST_PLUS
#define I2C4_GPIO_SPEED GPIO_SPEED_FREQ_HIGH
#endif

#ifndef I2C4_MODE
#error "I2C4_MODE is not defined. Please define it in hi2c4.h"
#endif



extern I2C_HandleTypeDef hi2c4;


void MX_I2C4_Init(void);