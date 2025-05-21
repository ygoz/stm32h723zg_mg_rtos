/**
 * @file utils.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-05-21
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#pragma once


#include <stdint.h>


#define I2C_MODE_STD 0
#define I2C_MODE_FAST 1
#define I2C_MODE_FAST_PLUS 2


typedef struct {
    uint8_t *data;
    uint16_t size;
} I2C_packet;