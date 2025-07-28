#pragma once


#include <stdbool.h>
#include "mongoose.h"
/* USER CODE BEGIN Private defines */
#define QSPI_FLASH_SECTION __attribute__((section(".extflash")))



#define W25Q_FLASH_SIZE                  			0x1000000 /* 128 MBits => 16MBytes  256x64x1024*/
#define W25Q_SECTOR_SIZE                 			0x10000   /* 64x1024 - 256 sectors of 64KBytes */
#define W25Q_SUBSECTOR_SIZE              			0x1000    /* 4*1024 = 4096 subsectors of 4kBytes */
#define W25Q_PAGE_SIZE                   			0x100     /* 65536 pages of 256 bytes */

#define W25Q_DUMMY_CYCLES_READ_QUAD      			6

#define W25Q_AUTOPOLLING_INTERVAL_TIME   			0x10U

/**
 *  W25Q128 Commands W25Q Chip's Commands
 */
#define W25Q_WRITE_ENABLE_CMD 						0x06U	/* sets WEL bit, must be set before any write/program/erase */
#define W25Q_ENABLE_VOLATILE_SR_CMD 				0x50U	/* check 7.1 in data sheet */
#define W25Q_WRITE_DISABLE_CMD 						0x04U	/* resets WEL bit (state after power-up) */

#define W25Q_POWERUP_CMD 							0xABU	/* release power-down */
#define W25Q_FULLID_CMD 							0x90U	/* read Manufacturer ID & Device ID */
#define W25Q_READ_JEDEC_ID_CMD 						0x9FU	/* read JEDEC-standard ID */
#define W25Q_READ_UID_CMD 							0x4BU	/* read unique chip 64-bit ID */

#define W25Q_READ_DATA_CMD 							0x03U	/* read data by standard SPI */
#define W25Q_FAST_READ_CMD 							0x0BU	/* highest FR speed (8.2.12) */

#define W25Q_PAGE_PROGRAM_CMD 						0x02U	/* program page (256bytes) by single SPI line */

#define W25Q_SECTOR_ERASE_CMD 						0x20U	/* sets all 4Kbyte sector with 0xFF (erases it) */
#define W25Q_32KB_BLOCK_ERASE_CMD 					0x52U	/* sets all 32Kbyte block with 0xFF */
#define W25Q_64KB_BLOCK_ERASE_CMD 					0xD8U	/* sets all 64Kbyte block with 0xFF */
#define W25Q_CHIP_ERASE_CMD 						0xC7U	/* fill all the chip with 0xFF */

#define W25Q_READ_SR1_CMD 							0x05U	/* read status-register 1 */
#define W25Q_WRITE_SR1_CMD 							0x01U	/* write status-register 1 (8.2.5) */
#define W25Q_READ_SR2_CMD 							0x35U	/* read status-register 2 */
#define W25Q_WRITE_SR2_CMD 							0x31U	/* write status-register 2 (8.2.5) */
#define W25Q_READ_SR3_CMD 							0x15U	/* read ststus-register 3 */
#define W25Q_WRITE_SR3_CMD 							0x11U	/* write status-register 3 (8.2.5) */

#define W25Q_READ_SFDP_CMD 							0x5AU	/* read SFDP register parameters */
#define W25Q_ERASE_SECURITY_REG_CMD					0x44U	/* erase security registers */
#define W25Q_PROG_SECURITY_REG_CMD 					0x42U	/* program security registers */
#define W25Q_READ_SECURITY_REG_CMD 					0x48U	/* read security registers */

#define W25Q_GLOBAL_LOCK_CMD 						0x7EU	/* global read-only protection enable */
#define W25Q_GLOBAL_UNLOCK_CMD 						0x98U	/* global read-only protection disable */
#define W25Q_READ_BLOCK_LOCK_CMD 					0x3DU	/* check block/sector protection */
#define W25Q_IND_BLOCK_LOCK_CMD 					0x36U	/* make block/sector read-only */
#define W25Q_IND_BLOCK_UNLOCK_CMD 					0x39U	/* disable block/sector protection */

#define W25Q_ERASEPROG_SUSPEND_CMD					0x75U	/* suspend erase/program operation (can be applied only when SUS=0, BYSY=1) */
#define W25Q_ERASEPROG_RESUME_CMD 					0x7AU	/* resume erase/program operation (if SUS=1, BUSY=0) */
#define W25Q_POWERDOWN_CMD 							0xB9U	/* powers down the chip (power-up by reading ID) */

#define W25Q_ENABLE_RST_CMD 						0x66U	/* enable software-reset ability */
#define W25Q_RESET_CMD 								0x99U	/* make software reset */

#define W25Q_FAST_READ_DUAL_OUT_CMD 				0x3BU	/* fast read in dual-SPI OUTPUT (8.2.14) */
#define W25Q_FAST_READ_DUAL_IO_CMD 					0xBBU	/* fast read in dual-SPI I/O (address transmits by both lines) */
#define W25Q_FULLID_DUAL_IO_CMD 					0x92U	/* read Manufacturer ID & Device ID by dual I/O */
#define W25Q_PAGE_PROGRAM_QUAD_INP_CMD 				0x32U	/* program page (256bytes) by Quad SPI lines */
#define W25Q_FAST_READ_QUAD_OUT_CMD 				0x6BU	/* fast read in quad-SPI OUTPUT (8.2.16) */
#define W25Q_FULLID_QUAD_IO_CMD 					0x94U	/* read Manufacturer ID & Device ID by quad I/O */
#define W25Q_FAST_READ_QUAD_IO_CMD 					0xEBU	/* fast read in quad-SPI I/O (address transmits by quad lines) */
#define W25Q_SET_BURST_WRAP_CMD 					0x77U	/* use with quad-I/O (8.2.22) */

/**
  *   N25Q128A Registers
  */


/* Status Register */
#define W25Q_SR_Quad_Enable							((uint8_t)0x02)    /*!< Quad Enable */
#define W25Q_SR_DRV1                     			((uint8_t)0x9f)    /*!< DRV1:2 bit */


// flash api
/**
 * @brief  Initializes the W25Q128 external flash.
 *         Configures flash registers, resets the chip, sets quad mode,
 *         and ensures device is ready.
 * @retval HAL_StatusTypeDef HAL_OK on success, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef w25q128_init(void);

/**
 * @brief  Writes data to the W25Q128 flash.
 * @param  pData Pointer to the buffer containing data to write.
 * @param  WriteAddr Destination address in flash memory.
 * @param  Size Number of bytes to write.
 * @retval HAL_StatusTypeDef HAL_OK on success, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef w25q128_write(uint8_t* pData, uint32_t WriteAddr, uint32_t Size);

/**
 * @brief  Reads data from the W25Q128 flash.
 * @param  pData Pointer to buffer to store read data.
 * @param  ReadAddr Source address in flash memory.
 * @param  Size Number of bytes to read.
 * @retval HAL_StatusTypeDef HAL_OK on success, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef w25q128_read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size);

/**
 * @brief  Enables memory-mapped mode for W25Q128.
 *         Allows direct access to flash content as if it were normal memory.
 *         Starts on 0x70000000
 * @retval HAL_StatusTypeDef HAL_OK on success, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef w25q128_memmap_enable(void);

/**
 * @brief  Disables memory-mapped mode for W25Q128.
 *         Required before performing write or erase operations.
 * @retval HAL_StatusTypeDef HAL_OK on success, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef w25q128_memmap_disable(void);

/**
 * @brief  Erases the entire W25Q128 flash chip.
 *         This is a time-consuming operation (several seconds).
 * @retval HAL_StatusTypeDef HAL_OK on success, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef w25q128_erase_chip(void);

/**
 * @brief  Erases one or more sectors in the W25Q128 flash.
 *         Each sector is 64 KB in size. The function erases all sectors
 *         starting at EraseStartAddress and continuing sequentially in
 *         64 KB increments up to and including EraseEndAddress.
 *
 * @param  EraseStartAddress Start address of the first sector to erase.
 * @param  EraseEndAddress   End address. All sectors up to this address
 *                           will be erased.
 * @retval HAL_StatusTypeDef HAL_OK on success, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef w25q128_erase_sector(uint32_t EraseStartAddress, uint32_t EraseEndAddress);


/**
 * @brief  Issues a reset command to the W25Q128 flash chip.
 * @retval HAL_StatusTypeDef HAL_OK on success, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef w25q128_reset_chip(void);

/**
 * @brief  Configures W25Q128 flash status registers.
 *
 * This function:
 *   - Reads SR1, SR2, and SR3.
 *   - Enables Quad-SPI mode by setting the QE bit in SR2.
 *   - Preserves drive-strength bits from SR3.
 *   - Writes updated values back to the status registers.
 *   - Verifies changes by re-reading the registers.
 *
 * @retval HAL_OK    Configuration completed successfully.
 * @retval HAL_ERROR Read/write operation failed.
 */
HAL_StatusTypeDef w25q128_config(void);


/**
 * @brief  Enables write operations on the W25Q128 flash.
 * @retval HAL_StatusTypeDef HAL_OK on success, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef w25q128_write_enable(void);

/**
 * @brief  Waits until the flash memory is ready using automatic polling.
 * @retval HAL_StatusTypeDef HAL_OK on success, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef w25q128_auto_polling_mem_ready(void);

/**
 * @brief  Checks if the flash is busy performing an operation.
 * @retval HAL_StatusTypeDef HAL_OK if ready, HAL_BUSY if busy, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef w25q128_is_busy(void);

/**
 * @brief  Reads a specific status register from W25Q128 flash.
 * @param  register_data Pointer to variable where the register value will be stored.
 * @param  register_num  Register number (1, 2, or 3).
 * @retval HAL_StatusTypeDef HAL_OK on success, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef w25q128_read_status_registers(uint8_t* register_data, uint8_t register_num);

/**
 * @brief  Writes to a specific status register of the W25Q128 flash.
 * @param  reg_data   Value to write.
 * @param  reg_num    Register number (1, 2, or 3).
 * @retval HAL_StatusTypeDef HAL_OK on success, HAL_ERROR otherwise.
 */
HAL_StatusTypeDef w25q128_write_status_registers(uint8_t reg_data, uint8_t reg_num);


// unit test
/**
 * @brief  Performs a full functional test of the W25Q128 flash.
 *
 * This test:
 *   - Initializes the flash interface.
 *   - Erases each sector sequentially.
 *   - Writes a known data pattern to each sector.
 *   - Enables memory-mapped mode.
 *   - Verifies the written data by direct memory-mapped reads.
 *
 * Prints diagnostic information via MG_INFO and serial output.
 *
 * @retval HAL_OK    Test passed successfully.
 * @retval HAL_ERROR Initialization, erase, write, or verification failed.
 */
HAL_StatusTypeDef w25q128_test(void);


// not implemented yet
HAL_StatusTypeDef w25q128_write_disable(void);
HAL_StatusTypeDef w25q128_erase_block_32KB(uint32_t BlockAddress);
HAL_StatusTypeDef w25q128_erase_block_64KB(uint32_t BlockAddress);


/**
 * @brief  Driver interface structure for QSPI flash devices.
 *
 * This structure defines function pointers for the essential operations
 * required to interact with a QSPI flash device. It allows switching
 * between different flash implementations by assigning device-specific
 * functions to the members.
 *
 * Members:
 *   - init            : Initializes the flash device.
 *   - write           : Writes data to the flash.
 *   - read            : Reads data from the flash.
 *   - memmap_enable   : Enables memory-mapped mode.
 *   - memmap_disable  : Disables memory-mapped mode.
 *   - erase_chip      : Erases the entire flash chip.
 *   - erase_sector    : Erases a range of sectors between start and end addresses.
 *
 * Example:
 * @code
 * if (w25q128_driver.init() == HAL_OK) {
 *     w25q128_driver.erase_chip();
 * }
 * @endcode
 */
typedef struct {
    HAL_StatusTypeDef (*init)(void);
    HAL_StatusTypeDef (*write)(uint8_t *pData, uint32_t write_addr, uint32_t size);
    HAL_StatusTypeDef (*read)(uint8_t *pData, uint32_t read_addr, uint32_t size);
    HAL_StatusTypeDef (*memmap_enable)(void);
    HAL_StatusTypeDef (*memmap_disable)(void);
    HAL_StatusTypeDef (*erase_chip)(void);
    HAL_StatusTypeDef (*erase_sector)(uint32_t erase_start_addr, uint32_t erase_end_addr);
} qspi_flash_driver_t;

/** 
 * @brief  W25Q128 flash driver implementing qspi_flash_driver_t interface.
 */
extern const qspi_flash_driver_t w25q128_driver;
