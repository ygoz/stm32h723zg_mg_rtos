/**
 ******************************************************************************
 * @file    i2c4.h
 * @brief   I2C4 initialization header
 *
 * Provides conditional initialization declarations for I2C4 based on the
 * selected I2C mode (Standard or Fast Mode).
 *
 * 🧩 Pin Mapping:
 *   SDA: PF15 
 *   SCL: PD12 
 *
 * ⚙️ Modes:
 *   - `I2C_STD`                to use Standard Mode (100 kHz)
 *
 *   - `I2C_FASTMODE`           to use Fast Mode (400 kHz)
 *
 *   - `I2C4_MODE_FAST_PLUS`    to use Fast Mode Plus (1000 kHz)
 * 
 * 
 * 💡 Usage:
 *   #define I2C_FASTMODE
 *   #include "i2c4.h"
 *   MX_I2C4_Init_FastMode();
 * 
 *  @protocol
 *  I2C is a two-wire serial communication system used between integrated circuits which was originally created by Philips Semiconductors back in 1982.
 *  The I2C is a multi-master, multi-slave, synchronous, bidirectional, half-duplex serial communication bus.
 *
 *  SDA (Serial Data) is the line on which master and slave send or receive the information (sequence of bits).
 *  SCL (Serial Clock) is the clock-dedicated line for data flow synchronization.
 *      
 *  Frame:
 *  -   Start Condition (S)
 *  -   Stop Condition (P)
 *  -   Repeated Start (Restart) Condition (Sr)
 *  -   Acknowledge ACK (A)
 *  -   Not Acknowledge NACK (/A)
 * 
 *  HAL SUPPORT:
 *  Based on STM32Cube HAL functions, I2C data transfer can be performed in 3 modes: 
 *  -   Blocking Mode -- this handle uses blocking mode because it writes / reads 
 *                       to / from an EEPROM so there is not reason to be interrupt based
 *  -   Interrupt Mode 
 *  -   DMA Mode
 *
 ******************************************************************************
 */
#pragma once

#include "stm32h7xx_hal.h" // needed for I2C_HandleTypeDef 
#include "main.h" // needed for error_handling
#include "serial_comm/i2c/utils.h"
#include "HaGeneral/HaGeneral_config.h"


#define I2C4_MODE_STD 0
#define I2C4_MODE_FAST 1
#define I2C4_MODE_FAST_PLUS 2



#ifndef I2C4_MODE
#error "I2C4_MODE is not defined. Please define it in HaGeneral_config.h (e.g., #define I2C4_MODE I2C4_MODE_STD)"
#endif

#if I2C4_MODE == I2C4_MODE_STD
#define I2C4_GPIO_SPEED GPIO_SPEED_FREQ_LOW
#elif I2C4_MODE == I2C4_MODE_FAST
#define I2C4_GPIO_SPEED GPIO_SPEED_FREQ_MEDIUM
#elif I2C4_MODE == I2C4_MODE_FAST_PLUS
#define I2C4_GPIO_SPEED GPIO_SPEED_FREQ_HIGH
#else
#error "Unsupported I2C4_MODE value. Use I2C4_MODE_STD, I2C4_MODE_FAST, or I2C4_MODE_FAST_PLUS."
#endif



extern I2C_HandleTypeDef hi2c4;


void MX_I2C4_Init(void);


HAL_StatusTypeDef I2C4_master_transmit(I2C_packet data_packet, uint16_t slave_addr);
  
HAL_StatusTypeDef I2C4_master_receive(I2C_packet buffer_packet, uint16_t slave_addr);

HAL_StatusTypeDef I2C4_mem_write(uint16_t mem_address, I2C_packet data_packet, uint16_t slave_addr);

HAL_StatusTypeDef I2C4_mem_read(uint16_t mem_address, I2C_packet buffer_packet, uint16_t slave_addr);