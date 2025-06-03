/**
 ******************************************************************************
 * @file    i2c1.h
 * @brief   I2C1 initialization header
 *
 * Provides conditional initialization declarations for I2C1 based on the
 * selected I2C mode (Standard or Fast Mode).
 *
 * 🧩 Pin Mapping:
 *   SDA: PB7 
 *   SCL: PB6 
 *
 * ⚙️ Modes:
 *   - `I2C_STD`                to use Standard Mode (100 kHz)
 *
 *   - `I2C_FASTMODE`           to use Fast Mode (400 kHz)
 *
 *   - `I2C_MODE_FAST_PLUS`    to use Fast Mode Plus (1000 kHz)
 * 
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





#ifndef I2C1_MODE
#error "I2C1_MODE is not defined. Please define it in HaGeneral_config.h (e.g., #define I2C1_MODE I2C1_MODE_STD)"
#endif

#if I2C1_MODE == I2C_MODE_STD
#define I2C1_GPIO_SPEED GPIO_SPEED_FREQ_LOW
#elif I2C1_MODE == I2C_MODE_FAST
#define I2C1_GPIO_SPEED GPIO_SPEED_FREQ_MEDIUM
#elif I2C1_MODE == I2C_MODE_FAST_PLUS
#define I2C1_GPIO_SPEED GPIO_SPEED_FREQ_HIGH
#else
#error "Unsupported I2C1_MODE value. Use I2C_MODE_STD, I2C_MODE_FAST, or I2C_MODE_FAST_PLUS."
#endif



extern I2C_HandleTypeDef hi2c1;


void MX_I2C1_Init(void);

/**
 * @brief  Transmits raw data over I2C1 in master mode (blocking).
 *
 * This function sends a buffer of bytes to a slave device using I2C1.
 * It uses STM32 HAL's `HAL_I2C_Master_Transmit()` in blocking mode and
 * introduces a short delay afterward to account for peripheral processing time
 * (such as EEPROM internal write delay).
 *
 * @param data_packet   A structure of type `I2C_packet` that contains:
 *                      - `data`: pointer to the byte array to transmit.
 *                      - `size`: number of bytes to send.
 * @param slave_addr    8-bit I2C address of the target device (7-bit left-shifted by 1).
 *                      For example: `0x50 << 1` gives `0xA0`.
 *
 * @return HAL status (`HAL_StatusTypeDef`) indicating success or failure.
 *
 * @note A delay of `I2C1_OP_DELAY` milliseconds is introduced after the transmission.
 *       This is helpful when communicating with EEPROMs or similar devices
 *       that require time to internally process the data.
 */
HAL_StatusTypeDef I2C1_master_transmit(I2C_packet data_packet, uint16_t slave_addr);
  

/**
 * @brief  Receives raw data over I2C1 in master mode (blocking).
 *
 * This function reads a specified number of bytes from a slave device using I2C1.
 * It uses STM32 HAL's `HAL_I2C_Master_Receive()` function in blocking mode.
 *
 * @param buffer_packet A structure of type `I2C_packet` that contains:
 *                      - `data`: pointer to the buffer to store received bytes.
 *                      - `size`: number of bytes to receive.
 * @param slave_addr    8-bit I2C address of the target device (7-bit left-shifted by 1).
 *                      For example: `0x50 << 1` gives `0xA0`.
 *
 * @return HAL status (`HAL_StatusTypeDef`) indicating success or failure.
 *
 * @note No delay is added after the receive operation, as most I2C devices
 *       do not require extra wait time after data reception.
 */
HAL_StatusTypeDef I2C1_master_receive(I2C_packet buffer_packet, uint16_t slave_addr);



/**
 * @brief  Writes data to an I2C memory device (e.g., EEPROM) at a specified memory address.
 *
 * This function performs a memory write transaction over I2C1 using blocking mode,
 * sending a memory address followed by the data bytes. It uses the STM32 HAL function
 * `HAL_I2C_Mem_Write()` with 16-bit internal memory addressing.
 *
 * @param mem_address   16-bit internal memory address in the target I2C device to begin writing to.
 * @param data_packet   A structure of type `I2C_packet` that contains:
 *                      - `data`: pointer to the data buffer to be written.
 *                      - `size`: number of bytes to write.
 * @param slave_addr    8-bit I2C address of the target device (7-bit left-shifted by 1).
 *                      For example: `0x50 << 1` gives `0xA0`.
 *
 * @return HAL status (`HAL_StatusTypeDef`) indicating success or error.
 *
 * @note A delay of `I2C1_OP_DELAY` milliseconds is added after the operation to accommodate
 *       EEPROM write cycle times, which can take several milliseconds internally.
 */
HAL_StatusTypeDef I2C1_mem_write(uint16_t mem_address, I2C_packet data_packet, uint16_t slave_addr);


/**
 * @brief  Performs a memory read over I2C using 16-bit addressing (typical EEPROM style).
 * 
 * This function reads a sequence of bytes from an I2C memory device, such as an EEPROM,
 * using the specified memory address and stores the result in the provided buffer.
 * 
 * It uses the STM32 HAL's `HAL_I2C_Mem_Read()` in blocking mode. The memory address is
 * 16-bit wide, suitable for EEPROMs with >256 bytes of memory.
 *
 * @param mem_address   16-bit internal memory address in the EEPROM to start reading from.
 * @param buffer_packet A structure of type `I2C_packet` that contains:
 *                      - `data`: a pointer to the buffer where the received data will be stored.
 *                      - `size`: number of bytes to read from the EEPROM.
 * @param slave_addr    The 8-bit I2C address of the target device (7-bit left-shifted by 1).
 *                      For example: `0x50 << 1` gives `0xA0`.
 *
 * @return HAL status (`HAL_StatusTypeDef`) indicating success or failure.
 *
 * @note No delay is inserted after read, as EEPROM reads typically do not require a wait period.
 */
HAL_StatusTypeDef I2C1_mem_read(uint16_t mem_address, I2C_packet buffer_packet, uint16_t slave_addr);