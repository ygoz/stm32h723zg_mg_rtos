/*
 * internal_flash.c
 *
 *  Created on: Apr 20, 2025
 *      Author: yam
 */



#include "stm32h7xx_hal.h"
#include "flash/internal_flash.h"
#include "string.h"
#include "stdio.h"
#include "mongoose.h"


#define FLASHWORD		8




// There are 2 BANKS available for H745, BANK 1 (0x0800 0000 - 0x080F FFFF) and BANK 2 (0x0810 0000 - 0x080F FFFF)
// Both of them have Sectors 0 to 7.
// We will define the sectors in normal way (like Defined below), and later the BANK will be taken care by the HAL

/**
 * @brief There are 2...    !!! put this in the header
 * @param Adress

 */
static uint32_t get_sector(uint32_t flash_addr)   // get_sector or GetSector in all the project.   variables only lower case
{
  uint32_t sector = 0;

  /* BANK 1 */
  if((flash_addr >= 0x08000000) && (flash_addr < 0x08020000))
  {
    sector = FLASH_SECTOR_0;
  }

  else if((flash_addr >= 0x08020000) && (flash_addr < 0x08040000))
  {
    sector = FLASH_SECTOR_1;
  }

  else if((flash_addr >= 0x08040000) && (flash_addr < 0x08060000))
  {
    sector = FLASH_SECTOR_2;
  }

  else if((flash_addr >= 0x08060000) && (flash_addr < 0x08080000))
  {
    sector = FLASH_SECTOR_3;
  }

  else if((flash_addr >= 0x08080000) && (flash_addr < 0x080A0000))
  {
    sector = FLASH_SECTOR_4;
  }

  else if((flash_addr >= 0x080A0000) && (flash_addr < 0x080C0000))
  {
    sector = FLASH_SECTOR_5;
  }

  else if((flash_addr >= 0x080C0000) && (flash_addr < 0x080E0000))
  {
    sector = FLASH_SECTOR_6;
  }

  else if((flash_addr >= 0x080E0000) && (flash_addr < 0x08100000))
  {
    sector = FLASH_SECTOR_7;
  }

  return sector;
}




uint8_t bytes_temp[4];  // put in header in the upper section of the file

// put documentation
void float2Bytes(uint8_t * ftoa_bytes_temp,float float_variable)
{
    union {
      float a;
      uint8_t bytes[4];
    } thing;

    thing.a = float_variable;

    for (uint8_t i = 0; i < 4; i++) {
      ftoa_bytes_temp[i] = thing.bytes[i];
    }

}

float Bytes2float(uint8_t * ftoa_bytes_temp)
{
    union {
      float a;
      uint8_t bytes[4];
    } thing;

    for (uint8_t i = 0; i < 4; i++) {
    	thing.bytes[i] = ftoa_bytes_temp[i];
    }

   float float_variable =  thing.a;
   return float_variable;
}




/* The DATA to be written here MUST be according to the List Shown Below

For EXAMPLE:- For H74x/5x, a single data must be 8 numbers of 32 bits word
If you try to write a single 32 bit word, it will automatically write 0's for the rest 7

*          - 256 bits for STM32H74x/5X devices (8x 32bits words)
*          - 128 bits for STM32H7Ax/BX devices (4x 32bits words)
*          - 256 bits for STM32H72x/3X devices (8x 32bits words)
*
*/

MG_IRAM uint32_t flash_write_data (uint32_t start_sector_addr, uint32_t *data, uint16_t num_of_words)  //  ==> num_words
{

	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t SECTORError;
	int words_so_far = 0;

	 /* Unlock the Flash to enable the flash control register access *************/
	  __disable_irq();
	  HAL_FLASH_Unlock();

	  /* Erase the user Flash area */

	  /* Get the number of sector to erase from 1st sector */

	  uint32_t start_sector = get_sector(start_sector_addr);
	  uint32_t end_sector_addr = start_sector_addr + num_of_words*4;
	  uint32_t end_sector = get_sector(end_sector_addr);

	  /* Fill EraseInit structure*/
	  EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
	  EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
	  EraseInitStruct.Sector        = start_sector;
	  EraseInitStruct.Banks     	= FLASH_BANK_1;
	  EraseInitStruct.NbSectors     = (end_sector - start_sector) + 1;


	  if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
	  {
		  return HAL_FLASH_GetError ();
	  }

	  /* Program the user Flash area 8 WORDS at a time ----------------pad zeros!!!!!!!!!!!!!!!!!!!!!!! dont write garbage
	   * (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

	   while (words_so_far < num_of_words)
	   {
	     if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, start_sector_addr, (uint32_t ) &data[words_so_far]) == HAL_OK)
	     {
	    	 start_sector_addr += 4 * FLASHWORD;  //
	    	 words_so_far+=FLASHWORD;
	     }
	     else
	     {
	       /* Error occurred while writing data in Flash memory*/
	    	 return HAL_FLASH_GetError ();
	     }
	   }

	  /* Lock the Flash to disable the flash control register access (recommended
	     to protect the FLASH memory against possible unwanted operation) *********/
	  HAL_FLASH_Lock();
	  __enable_irq();

	   return 0;
}


// ** THIS CAUSES HARDFAULT :( stupid controllerstech **
//MG_IRAM void flash_read_data (uint32_t StartSectorAddress, uint32_t *data, uint16_t numberofwords) {
//	while (1) {
//		*data = *(__IO uint32_t *)StartSectorAddress;
//		StartSectorAddress += 4;
//		data++;
//		if (!(numberofwords--)) break;
//	}
//}





 MG_IRAM uint8_t flash_read_data(uint32_t start_sector_addr, uint32_t *data, uint16_t num_of_words) {
    uint32_t end_addr = start_sector_addr + num_of_words * 4;


	// To avoid hardfaults, make sure flash addr exists withing flash 'borders'
	// also, because you are reading 32-bit values (uint32_t) from Flash, 
	// the memory address must be aligned to 4 bytes. On most ARM Cortex-M MCUs, 
	// misaligned 32-bit reads can cause hard faults
    if (start_sector_addr < INTERNAL_FLASH_BASE_ADDRESS ||
        end_addr > INTERNAL_FLASH_END_ADDRESS ||
        start_sector_addr % 4 != 0 ) {
        return FLASH_READ_INVALID;
    }

    __disable_irq();
    for (uint16_t i = 0; i < num_of_words; i++) {
        data[i] = *(__IO uint32_t *)(start_sector_addr + i * 4);
    }
    __enable_irq();
    return FLASH_READ_OK;
}

// OLD METHOD!!!!!
// MG_IRAM void Flash_Read_Data (uint32_t StartSectorAddress, uint32_t *data, uint16_t numberofwords) {
// 	__disable_irq();
// 	for (uint16_t i = 0; i < numberofwords; i++) {
// 		data[i] = *(__IO uint32_t *)(StartSectorAddress + i * 4);
// 	}
//     __enable_irq();

// }








void Convert_To_Str (uint32_t *Data, char *Buf)
{
	int numberofbytes = ((strlen((char *)Data)/4) + ((strlen((char *)Data) % 4) != 0)) *4;

	for (int i=0; i<numberofbytes; i++)
	{
		Buf[i] = Data[i / 4] >> (8 * (i % 4));
	}
}


MG_IRAM void Flash_Write_NUM (uint32_t StartSectorAddress, float Num)
{

	float2Bytes(bytes_temp, Num);
	flash_write_data (StartSectorAddress, (uint32_t *)bytes_temp, 1);
}


MG_IRAM float Flash_Read_NUM (uint32_t StartSectorAddress)
{
	uint8_t buffer[4];
	float value;

	flash_read_data(StartSectorAddress, (uint32_t *)buffer, 1);
	value = Bytes2float(buffer);
	return value;
}
