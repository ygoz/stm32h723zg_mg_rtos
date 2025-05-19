#pragma once
#include <stdint.h>

typedef struct {
    uint8_t *data;
    uint16_t size;
} I2C_packet;