/*
 * internal_flash.h
 *
 *  Created on: Apr 20, 2025
 *      Author: yam
 */


#ifndef FLASH_INTERNAL_FLASH
#define FLASH_INTERNAL_FLASH

#include "stm32h7xx_hal.h"

// doucment all the functions here
uint32_t Flash_Write_Data (uint32_t StartSectorAddress, uint32_t *data, uint16_t numberofwords);
void Flash_Read_Data (uint32_t StartSectorAddress, uint32_t *data, uint16_t numberofwords);
void Convert_To_Str (uint32_t *Data, char *Buf);

void Flash_Write_NUM (uint32_t StartSectorAddress, float Num);
float Flash_Read_NUM (uint32_t StartSectorAddress);


#endif /* FLASH_INTERNAL_FLASH */
