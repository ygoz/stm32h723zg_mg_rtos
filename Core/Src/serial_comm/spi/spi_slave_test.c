#include "main.h"
#include <stdio.h>
#include "HaGeneral/HaGeneral_config.h"
#include "serial_comm/spi/hspi4.h"
#include "serial_comm/spi/hspi5.h"
#include "serial_comm/spi/spi_hil_test.h"


#ifdef HIL_SLAVE_MODE



static uint8_t tx_buffer[] = SPI_SLAVE_RESPONSE;
static uint8_t rx_buffer[SPI_BUFF_SIZE] = {0};





#endif