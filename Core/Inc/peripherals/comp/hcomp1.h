/**
 * @file hcomp1.h
 * @author Yam Goz (you@domain.com)
 * @brief Header file for COMP1 handle configuration and pin mapping.
 * @version 0.1
 * @date 2025-06-23
 * 
 * @copyright Copyright (c) 2025
 * 
 * @details
 * This file provides the configuration for the COMP1 peripheral, including its modes and pin assignments.
 * - Comparator modes: COMP1_MODE_OFF, COMP1_MODE_POLLING, COMP1_MODE_IT
 * - Hysteresis levels: COMP1_HYSTERESIS_NONE, LOW, MEDIUM, HIGH
 * - Trigger modes: Rising edge, Falling edge, Rising and Falling edges
 * - Pin mappings:
 *     - PB0: COMP1 positive input (INP)
 *     - PB1: COMP1 negative input (INM)
 * - Comparator output can be read via HAL API and used for threshold detection or interrupts.
 */


#pragma once

#include "stm32h7xx_hal.h"
#include "HaGeneral/HaGeneral_config.h"
#include <stdint.h>
#include "mongoose.h"
#include <stm32h7xx_hal_comp.h>


// ============================
// COMP1 Configuration Checks
// ============================

// ---------- Mode ----------
#ifndef COMP1_MODE
#error "COMP1_MODE is not defined. Please define it in HaGeneral_config.h (e.g., #define COMP1_MODE COMP1_MODE_POLLING)"
#endif

#if (COMP1_MODE != COMP1_MODE_POLLING) && (COMP1_MODE != COMP1_MODE_IT)
#error "Invalid COMP1_MODE value. Allowed values: COMP1_MODE_POLLING or COMP1_MODE_IT"
#endif

// ---------- Hysteresis ----------
#ifndef COMP1_HYSTERESIS_MODE
#error "COMP1_HYSTERESIS_MODE is not defined. Please define it in HaGeneral_config.h (e.g., #define COMP1_HYSTERESIS_MODE COMP1_HYSTERESIS_LOW)"
#endif

#if (COMP1_HYSTERESIS_MODE != COMP1_HYSTERESIS_NONE) && \
    (COMP1_HYSTERESIS_MODE != COMP1_HYSTERESIS_LOW)  && \
    (COMP1_HYSTERESIS_MODE != COMP1_HYSTERESIS_MEDIUM) && \
    (COMP1_HYSTERESIS_MODE != COMP1_HYSTERESIS_HIGH)
#error "Invalid COMP1_HYSTERESIS_MODE value. Allowed values: COMP1_HYSTERESIS_NONE, _LOW, _MEDIUM, _HIGH"
#endif

// ---------- Trigger Mode ----------
#ifndef COMP1_TRIGGER_MODE
#error "COMP1_TRIGGER_MODE is not defined. Please define it in HaGeneral_config.h (e.g., #define COMP1_TRIGGER_MODE COMP1_TRIGGER_MODE_RISING)"
#endif

#if (COMP1_TRIGGER_MODE != COMP1_TRIGGER_MODE_RISING) && \
    (COMP1_TRIGGER_MODE != COMP1_TRIGGER_MODE_FALLING) && \
    (COMP1_TRIGGER_MODE != COMP1_TRIGGER_MODE_RISING_FALLING)
#error "Invalid COMP1_TRIGGER_MODE value. Allowed values: COMP1_TRIGGER_MODE_RISING, _FALLING, or _RISING_FALLING"
#endif



// Comparator operating modes
#define COMP1_MODE_OFF      0   ///< Comparator disabled
#define COMP1_MODE_POLLING  1   ///< Polling mode: Read comparator output in software
#define COMP1_MODE_IT       2   ///< Interrupt mode: Trigger callback on edge detection

// Comparator trigger modes
#define COMP1_TRIGGER_MODE_RISING          COMP_TRIGGERMODE_IT_RISING           ///< Interrupt on rising edge
#define COMP1_TRIGGER_MODE_FALLING         COMP_TRIGGERMODE_IT_FALLING          ///< Interrupt on falling edge
#define COMP1_TRIGGER_MODE_RISING_FALLING  COMP_TRIGGERMODE_IT_RISING_FALLING   ///< Interrupt on both edges

// Comparator hysteresis levels
#define COMP1_HYSTERESIS_NONE    COMP_HYSTERESIS_NONE    ///< No hysteresis
#define COMP1_HYSTERESIS_LOW     COMP_HYSTERESIS_LOW     ///< Low hysteresis
#define COMP1_HYSTERESIS_MEDIUM  COMP_HYSTERESIS_MEDIUM  ///< Medium hysteresis
#define COMP1_HYSTERESIS_HIGH    COMP_HYSTERESIS_HIGH    ///< High hysteresis




extern COMP_HandleTypeDef hcomp1;


void MX_COMP1_Init(void);


/**
 * @brief Comparator trigger callback.
 *
 * This function is a callback function called by the HAL driver when a comparator trigger event occurs.
 * It is used by the user to handle rising/falling edge interrupts or other comparator-based events.
 *
 * @note This function is only relevant when the comparator is configured in interrupt mode
 *       (e.g., using COMP1_MODE_IT and an appropriate trigger mode such as rising/falling).
 *
 * @param hcomp Pointer to the comparator handle structure (e.g., &hcomp1).
 */
void HAL_COMP_TriggerCallback(COMP_HandleTypeDef *hcomp);


/**
 * @brief Starts the comparator and retrieves its output level.
 *
 * This function starts the COMP1 peripheral based on the configured mode
 * (polling or interrupt). It then reads the output level of the comparator
 * and stores it in the provided output pointer.
 *
 * - If `COMP1_MODE` is `COMP1_MODE_POLLING`, it calls `HAL_COMP_Start()`.
 * - If `COMP1_MODE` is `COMP1_MODE_IT`, it calls `HAL_COMP_Start_IT()`.
 * - If `COMP1_MODE` is `COMP1_MODE_OFF`, it logs a message and returns an error.
 *
 * The output level is one of the following values:
 * - `COMP_OUTPUT_LEVEL_HIGH`
 * - `COMP_OUTPUT_LEVEL_LOW`
 *
 * @param[out] comp1_output Pointer to a variable where the comparator output level will be stored.
 *                          Must not be NULL.
 * 
 * @retval HAL_OK      Comparator started successfully and output level was read.
 * @retval HAL_ERROR   Invalid pointer or comparator start failure.
 */
HAL_StatusTypeDef comp1_get_value(uint32_t *comp1_output);