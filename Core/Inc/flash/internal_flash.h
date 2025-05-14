/*
 * internal_flash.h
 *
 *  Created on: Apr 20, 2025
 *      Author: yam
 */


#ifndef FLASH_INTERNAL_FLASH
#define FLASH_INTERNAL_FLASH

#include "stm32h7xx_hal.h"


#define FLASH_READ_OK                   0x00
#define FLASH_READ_ERR                  0x01
#define FLASH_READ_INVALID              0x02  // For bad address/length
#define INTERNAL_FLASH_BASE_ADDRESS     0x08000000U  // flash base address
#define INTERNAL_FLASH_END_ADDRESS      0x080FFFFFU  // end of flash -> 1MB


// doucment all the functions here


/**
 * @brief  Erases flash sectors and writes an array of 32-bit words to flash memory.
 *
 * This function first erases all flash sectors covering the address range
 * starting from `StartSectorAddress` and spanning `numberofwords` 32-bit words.
 * Then it writes the data in chunks of 8 words (a flash "word" is 64 bytes on STM32H7).
 *
 * @note   - Interrupts are disabled during flash operations and re-enabled after.
 *         - Flash must be unlocked before programming; this function handles unlock/lock.
 *         - Erase is **automatic** for all affected sectors.
 *         - The data is written using `FLASH_TYPEPROGRAM_FLASHWORD`, which writes 64 bytes at a time.
 *         - `start_sector_addr` must be aligned to 64-byte boundaries.
 *
 * @param  start_sector_addr  Start address in flash memory (must be 64-byte aligned).
 * 
 * @param  data                Pointer to the data buffer to write (array of 32-bit words).
 * 
 * @param  num_of_words       Number of 32-bit words to write. Must be a multiple of 8.
 *
 * @retval uint32_t
 *         - 0 on success
 *         - HAL error code on failure (e.g., from `HAL_FLASHEx_Erase` or `HAL_FLASH_Program`)
 * 
 * @warning
 * 
 *	The DATA to be written here MUST be according to the List Shown Below
 *	For EXAMPLE:- For H72x/3x, a single data must be 8 numbers of 32 bits word
 *	If you try to write a single 32 bit word, it will automatically write 0's for the rest 7
 *
 *		- 256 bits for STM32H72x/3X devices (8x 32bits words)
 * 
 *
 * @example
 * @code
 * network_settings *data = ...
 * uint16_t num_words = sizeof(network_settings) / 4 + (sizeof(network_settings) % 4 != 0);
 * if (flash_write_data(0x08080000, (uint32_t *)data, num_words) == 0) {
 *     // Successfully written
 * } else {
 *     // Handle error
 * }
 * @endcode
 */
uint32_t flash_write_data (uint32_t start_sector_addr, uint32_t *data, uint16_t num_of_words);




/**
 * @brief  Reads a sequence of 32-bit words from Flash memory into RAM.
 *
 * This function reads `numberofwords` 32-bit values starting at the given
 * flash memory address (`StartSectorAddress`) and stores them into the
 * provided buffer (`data`). Flash is read as normal memory using direct
 * pointer dereferencing.
 *
 * @note   This function disables interrupts during the read operation to
 *         ensure consistency in time-critical applications. In most STM32
 *         MCUs, reading flash does not require unlocking or special access
 *         permissions.
 *
 * @param  start_sector_addr  The starting Flash memory address to read from.
 *                             Must be aligned to a 32-bit word boundary.
 *
 * @param  data                Pointer to a buffer in RAM where the read
 *                             data will be stored.
 *
 * @param  num_of_words       Number of 32-bit words to read from flash.
 *
 * @retval status, one of:    FLASH_READ_OK
 *                            FLASH_READ_ERR 
 *                            FLASH_READ_INVALID
 */
uint8_t flash_read_data(uint32_t start_sector_addr, uint32_t *data, uint16_t num_of_words);




// currently unused method
// void Convert_To_Str (uint32_t *Data, char *Buf);
// void Flash_Write_NUM (uint32_t StartSectorAddress, float Num);
// float Flash_Read_NUM (uint32_t StartSectorAddress);


#endif /* FLASH_INTERNAL_FLASH */
