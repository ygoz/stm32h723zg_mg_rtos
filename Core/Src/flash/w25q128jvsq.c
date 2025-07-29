#include <string.h>
#include <stdio.h>
#include "stm32h7xx_hal.h"
#include "serial_comm/spi/octospi.h"
#include "flash/w25q128jvsq.h"



const qspi_flash_driver_t w25q128_driver = {
    .init                   = w25q128_init,
    .write                  = w25q128_write,
    .read                   = w25q128_read,
    .memmap_enable          = w25q128_memmap_enable,
    .memmap_disable         = w25q128_memmap_disable,
    .erase_chip             = w25q128_erase_chip,
    .erase_sector           = w25q128_erase_sector,
    .reset_chip             = w25q128_reset_chip,
    .read_status_registers  = w25q128_read_status_registers,
    .write_status_registers = w25q128_write_status_registers,
    .test                   = w25q128_test
};





static void ospi_command_default_init(OSPI_RegularCmdTypeDef *sCommand) {
    sCommand->OperationType             = HAL_OSPI_OPTYPE_COMMON_CFG;             // Common configuration (indirect or auto-polling mode)
    sCommand->FlashId                   = HAL_OSPI_FLASH_ID_1;                    // Use Flash ID 1
    sCommand->InstructionDtrMode        = HAL_OSPI_INSTRUCTION_DTR_DISABLE;       // Disable Instruction DDR/DTR Mode
    sCommand->AddressDtrMode            = HAL_OSPI_ADDRESS_DTR_DISABLE;           // Disable Address DDR/DTR Mode
    sCommand->DataDtrMode               = HAL_OSPI_DATA_DTR_DISABLE;              // Disable Data DDR/DTR Mode
    sCommand->DQSMode                   = HAL_OSPI_DQS_DISABLE;                   // Disable Data Strobe (DQS)
    sCommand->SIOOMode                  = HAL_OSPI_SIOO_INST_EVERY_CMD;           // Send instruction on every command
    sCommand->AlternateBytesMode        = HAL_OSPI_ALTERNATE_BYTES_NONE;          // No alternate bytes
    sCommand->AlternateBytes            = HAL_OSPI_ALTERNATE_BYTES_NONE;          // Alternate bytes = 0
    sCommand->AlternateBytesSize        = HAL_OSPI_ALTERNATE_BYTES_NONE;          // Size = 0
    sCommand->AlternateBytesDtrMode     = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;   // Disable alternate bytes DTR mode
    sCommand->InstructionMode           = HAL_OSPI_INSTRUCTION_1_LINE;            // Use 1 line for instruction
    sCommand->InstructionSize           = HAL_OSPI_INSTRUCTION_8_BITS;            // 8-bit instruction
    sCommand->AddressSize               = HAL_OSPI_ADDRESS_24_BITS;               // 24-bit address
}



/* OCTO SPI Initial Function */
HAL_StatusTypeDef w25q128_init(void)
{
	if (HAL_OSPI_DeInit(&hospi2) != HAL_OK) {
	    return HAL_ERROR;
	}

	MX_OCTOSPI2_Init();

	if (w25q128_reset_chip() != HAL_OK) {
	    return HAL_ERROR;
	}
	if (w25q128_config() != HAL_OK) {
        return HAL_ERROR;
    }
	HAL_Delay(1);
	if (w25q128_auto_polling_mem_ready() != HAL_OK) {
        return HAL_ERROR;
    }
    if (w25q128_write_enable() != HAL_OK) {
        return HAL_ERROR;
    }
    return HAL_OK;
}

/* Reset Chip Function */
HAL_StatusTypeDef w25q128_reset_chip(void)
{
    OSPI_RegularCmdTypeDef sCommand={0};

    /* Enable Reset --------------------------- */
	/* Common Commands*/
    ospi_command_default_init(&sCommand);
	/* Instruction */
	sCommand.Instruction 				= W25Q_ENABLE_RST_CMD;						/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
	sCommand.Address					= 0;										/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_NONE;						/* Define Data Lines: No Data */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= 0;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    /* Reset Device --------------------------- */
	/* Common Commands*/
    ospi_command_default_init(&sCommand);
	/* Instruction */
	sCommand.Instruction 				= W25Q_RESET_CMD;							/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
	sCommand.Address					= 0;										/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_NONE;						/* Define Data Lines: No Data */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= 0;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }
    return HAL_OK;
}

/* Enable Quad Mode & Set Dummy Cycles Count */
HAL_StatusTypeDef w25q128_config(void)
{
    uint8_t reg1=0, reg2=0, reg3=0;
    uint8_t w_reg1=0, w_reg2=0, w_reg3=0;

    if (w25q128_read_status_registers(&reg1, 1) != HAL_OK)
        return HAL_ERROR;
    if (w25q128_read_status_registers(&reg2, 2) != HAL_OK)
        return HAL_ERROR;
    if (w25q128_read_status_registers(&reg3, 3) != HAL_OK)
        return HAL_ERROR;

    w_reg1 = 0;
    w_reg2 = W25Q_SR_Quad_Enable;
    w_reg3 = (reg3 & W25Q_SR_DRV1);

    if (w25q128_write_status_registers(w_reg1, 1) != HAL_OK)
        return HAL_ERROR;
    if (w25q128_write_status_registers(w_reg2, 2) != HAL_OK)
        return HAL_ERROR;
    if (w25q128_write_status_registers(w_reg3, 3) != HAL_OK)
        return HAL_ERROR;

    /* 
    HAL_OK means the read or write op was completed but
    the register values may not change, read again to verify
    that the target values has been set
    */
    if (w25q128_read_status_registers(&reg1, 1) != HAL_OK)
        return HAL_ERROR;
    if (w25q128_read_status_registers(&reg2, 2) != HAL_OK)
        return HAL_ERROR;
    if (w25q128_read_status_registers(&reg3, 3) != HAL_OK)
        return HAL_ERROR;

    MG_INFO(("Status Registers AFTER configuration:\r\n\t\t\tSR1: 0x%02X, SR2: 0x%02X, SR3: 0x%02X", reg1, reg2, reg3));

    return HAL_OK;
}

/* Write Enable Function */
HAL_StatusTypeDef w25q128_write_enable(void)
{
    OSPI_RegularCmdTypeDef sCommand;
    OSPI_AutoPollingTypeDef sConfig;

    /* Enable write operations ------------------------------------------ */
	/* Common Commands*/
    ospi_command_default_init(&sCommand);
	/* Instruction */
	sCommand.Instruction 				= W25Q_WRITE_ENABLE_CMD;					/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
	sCommand.Address					= 0;										/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_NONE;						/* Define Data Lines: No Data */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= 0;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

	/* Common Commands*/
    ospi_command_default_init(&sCommand);
	/* Instruction */
	sCommand.Instruction 				= W25Q_READ_SR1_CMD;						/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
	sCommand.Address					= 0;										/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_1_LINE;						/* Define Data Lines: Data On a Single Line */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= 1;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    /* Configure automatic polling mode to wait for write enabling ---- */
    sConfig.Match 					= 0x02U;
    sConfig.Mask 					= 0x02U;
    sConfig.MatchMode 				= HAL_OSPI_MATCH_MODE_AND;
    sConfig.Interval 				= W25Q_AUTOPOLLING_INTERVAL_TIME;
    sConfig.AutomaticStop 			= HAL_OSPI_AUTOMATIC_STOP_ENABLE;


    if (HAL_OSPI_AutoPolling(&hospi2, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/* Auto Polling Memory Function */
HAL_StatusTypeDef w25q128_auto_polling_mem_ready(void)
{

    OSPI_RegularCmdTypeDef sCommand;
    OSPI_AutoPollingTypeDef sConfig;

    /* Configure automatic polling mode to wait for memory ready ------ */
	/* Common Commands*/
    ospi_command_default_init(&sCommand);
	/* Instruction */
	sCommand.Instruction 				= W25Q_READ_SR1_CMD;						/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
	sCommand.Address					= 0;										/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_1_LINE;						/* Define Data Lines: Data On a Single Line */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= 1;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    sConfig.Match           			= 0x00U;
    sConfig.Mask            			= 0x01U;
    sConfig.MatchMode       			= HAL_OSPI_MATCH_MODE_AND;
    sConfig.Interval        			= W25Q_AUTOPOLLING_INTERVAL_TIME;
    sConfig.AutomaticStop   			= HAL_OSPI_AUTOMATIC_STOP_ENABLE;

    if (HAL_OSPI_AutoPolling(&hospi2, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/* Erase Chip Function */
HAL_StatusTypeDef w25q128_erase_chip(void)
{
    OSPI_RegularCmdTypeDef sCommand={0};
    //uint8_t reg3=0, w_reg3=0;

    /* Erasing Sequence ---------------------------------*/
	/* Common Commands*/
    ospi_command_default_init(&sCommand);
	/* Instruction */
	sCommand.Instruction 				= W25Q_CHIP_ERASE_CMD;						/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
	sCommand.Address					= 0;										/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_NONE;						/* Define Data Lines: No Data */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= 1;										/* Bytes Send With Data */

    if (w25q128_write_enable() != HAL_OK) {
        return HAL_ERROR;
    }

    if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    while (w25q128_is_busy()==HAL_ERROR)
    {
    	HAL_Delay(1);
    }

    if (w25q128_auto_polling_mem_ready() != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}

/* Erase Sector Function */
HAL_StatusTypeDef w25q128_erase_sector(uint32_t EraseStartAddress, uint32_t EraseEndAddress)
{
    OSPI_RegularCmdTypeDef sCommand={0};
    uint32_t StartAddress=0;

    StartAddress = EraseStartAddress - (EraseStartAddress % W25Q_SECTOR_SIZE);

    while (EraseEndAddress >= StartAddress)
    {
    	/* Erasing Sequence -------------------------------------------------- */
    	/* Common Commands*/
        ospi_command_default_init(&sCommand);
    	/* Instruction */
    	sCommand.Instruction 				= W25Q_64KB_BLOCK_ERASE_CMD;				/* What We Do? */
        // sCommand.Instruction 				= W25Q_SECTOR_ERASE_CMD;
    	/* Address */
    	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_1_LINE;					/* Define Address Lines: Address On a Single Line */
    	sCommand.Address					= (StartAddress & 0xFFFFFF);				/* Byte Address */
    	/* Data */
    	sCommand.DataMode          			= HAL_OSPI_DATA_NONE;						/* Define Data Lines: No Data */
    	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
    	sCommand.NbData            			= 0;										/* Bytes Send With Data */

        if (w25q128_write_enable() != HAL_OK) {
            return HAL_ERROR;
        }

        if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            return HAL_ERROR;
        }

        if (w25q128_auto_polling_mem_ready() != HAL_OK) {
            return HAL_ERROR;
        }

        StartAddress += W25Q_SECTOR_SIZE;
    }

    return HAL_OK;
}

/* Write Function */
HAL_StatusTypeDef w25q128_write(const uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
{
  OSPI_RegularCmdTypeDef sCommand={0};
  uint32_t end_addr=0, current_size=0, current_addr=0;
  uint32_t data_addr=0;

  current_addr = 0;

  while (current_addr <= WriteAddr) {
      current_addr += W25Q_PAGE_SIZE;
  }
  current_size = current_addr - WriteAddr;

  /* Check if the size of the data is less than the remaining place in the page */
  if (current_size > Size) {
      current_size = Size;
  }

  /* Initialize the adress variables */
  current_addr = WriteAddr;
  end_addr = WriteAddr + Size;

  data_addr = (uint32_t)pData;

  /* Perform the write page by page */
  do
  {
	/* Initialize the program command */
	/* Common Commands*/
    ospi_command_default_init(&sCommand);
	/* Instruction */
	sCommand.Instruction 				= W25Q_PAGE_PROGRAM_QUAD_INP_CMD;			/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_1_LINE;					/* Define Address Lines: Address On a Single Line */
	sCommand.Address					= current_addr;								/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_4_LINES;					/* Define Data Lines: Data On Four Lines */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= current_size;								/* Bytes Send With Data */

    if (current_size == 0) {
        return HAL_OK;
    }

    /* Enable write operations */
    if (w25q128_write_enable() != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Configure the command */
    if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Transmission of the data */
    if (HAL_OSPI_Transmit(&hospi2, (uint8_t*)data_addr, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Configure automatic polling mode to wait for end of program */
    if (w25q128_auto_polling_mem_ready() != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Update the address and size variables for next page programming */
    current_addr += current_size;
    data_addr += current_size;
    current_size = ((current_addr + W25Q_PAGE_SIZE) > end_addr) ? (end_addr - current_addr) : W25Q_PAGE_SIZE;
  } while (current_addr <= end_addr);

  return HAL_OK;
}

/* Read Function */
HAL_StatusTypeDef w25q128_read(uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
  OSPI_RegularCmdTypeDef sCommand={0};
  /* Initialize the read command */
  /* Common Commands*/
  ospi_command_default_init(&sCommand);
  /* Instruction */
  sCommand.Instruction 				= W25Q_FAST_READ_QUAD_IO_CMD;				/* What We Do? */
  /* Address */
  sCommand.AddressMode       		= HAL_OSPI_ADDRESS_4_LINES;					/* Define Address Lines: Address On Four Line */
  sCommand.Address					= ReadAddr;									/* Byte Address */
  /* Data */
  sCommand.DataMode          		= HAL_OSPI_DATA_4_LINES;					/* Define Data Lines: Data On Four Lines */
  sCommand.DummyCycles       		= W25Q_DUMMY_CYCLES_READ_QUAD;										/* Bytes Send With No Data */
  sCommand.NbData            		= Size;										/* Bytes Send With Data */

  /* Configure the command */
  if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reception of the data */
  if (HAL_OSPI_Receive(&hospi2, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}

/* Memory Map Enable Function */
HAL_StatusTypeDef w25q128_memmap_enable(void)
{

    OSPI_RegularCmdTypeDef sCommand={0};
    OSPI_MemoryMappedTypeDef sMemMappedCfg={0};

    /* Enable Memory-Mapped mode-------------------------------------------------- */
	/* Common Commands*/
    sCommand.OperationType      	= HAL_OSPI_OPTYPE_READ_CFG; 				/* Read Configuration (Memory-Mapped Mode) */
    sCommand.FlashId            	= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
    sCommand.InstructionDtrMode 	= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
    sCommand.AddressDtrMode     	= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
    sCommand.DataDtrMode			= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
    sCommand.DQSMode            	= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
    sCommand.SIOOMode          		= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
    sCommand.AlternateBytesMode 	= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
    sCommand.AlternateBytes			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
    sCommand.AlternateBytesSize		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
    sCommand.AlternateBytesDtrMode	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
    sCommand.InstructionMode   		= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
    sCommand.InstructionSize    	= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
    sCommand.AddressSize 			= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
    /* Instruction */
    sCommand.Instruction 			= W25Q_FAST_READ_QUAD_IO_CMD;				/* What We Do? */
    /* Address */
    sCommand.AddressMode       		= HAL_OSPI_ADDRESS_4_LINES;					/* Define Address Lines: Address On Four Lines */
    sCommand.Address				= 0;										/* Byte Address */
    /* Data */
    sCommand.DataMode          		= HAL_OSPI_DATA_4_LINES;					/* Define Data Lines: Data On Four Lines */
    sCommand.DummyCycles       		= W25Q_DUMMY_CYCLES_READ_QUAD;				/* Bytes Send With No Data */
    sCommand.NbData            		= 0;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    /* Initialize the program command */
	/* Common Commands*/
    sCommand.OperationType      	= HAL_OSPI_OPTYPE_WRITE_CFG; 				/* Write Configuration (Memory-Mapped Mode)) */
    sCommand.FlashId            	= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
    sCommand.InstructionDtrMode 	= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
    sCommand.AddressDtrMode     	= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
    sCommand.DataDtrMode			= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
    sCommand.DQSMode            	= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
    sCommand.SIOOMode          		= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
    sCommand.AlternateBytesMode 	= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
    sCommand.AlternateBytes			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
    sCommand.AlternateBytesSize		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
    sCommand.AlternateBytesDtrMode	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
    sCommand.InstructionMode   		= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
    sCommand.InstructionSize    	= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
    sCommand.AddressSize 			= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
    /* Instruction */
    sCommand.Instruction 			= W25Q_PAGE_PROGRAM_QUAD_INP_CMD;			/* What We Do? */
    /* Address */
    sCommand.AddressMode       		= HAL_OSPI_ADDRESS_1_LINE;					/* Define Address Lines: Address On a Single Line */
    sCommand.Address				= 0;										/* Byte Address */
    /* Data */
    sCommand.DataMode          		= HAL_OSPI_DATA_4_LINES;					/* Define Data Lines: Data On Four Lines */
    sCommand.DummyCycles       		= 0;										/* Bytes Send With No Data */
    sCommand.NbData            		= 0;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    /* Initialize Memory Mapped Command */
    sMemMappedCfg.TimeOutActivation 	= HAL_OSPI_TIMEOUT_COUNTER_DISABLE;		/* Timeout counter disabled, nCS remains active */

    if (HAL_OSPI_MemoryMapped(&hospi2, &sMemMappedCfg) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}


HAL_StatusTypeDef w25q128_memmap_disable(void)
{
    // Abort memory-mapped mode
    if (HAL_OSPI_Abort(&hospi2) != HAL_OK) {
        return HAL_ERROR;
    }

    // Reinitialize the OSPI peripheral
    if (w25q128_init() != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}



/* Check Chip is Busy Function */
HAL_StatusTypeDef w25q128_is_busy(void)
{
	HAL_StatusTypeDef state;
	uint8_t status_rgister = {0};

	state = w25q128_read_status_registers(&status_rgister, 1);
	if (state != HAL_OK)
		return state;

	status_rgister = status_rgister & 0b1;

	return status_rgister ? HAL_ERROR : HAL_OK;
}

/* Read Status Registers Function */
HAL_StatusTypeDef w25q128_read_status_registers(uint8_t* register_data, uint8_t register_num)
{
	OSPI_RegularCmdTypeDef sCommand={0};

	/* Common Commands*/
    ospi_command_default_init(&sCommand);
    /* Instruction */
    sCommand.Instruction 			= 0;										/* What We Do? */
    /* Address */
    sCommand.AddressMode       		= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
    sCommand.Address				= 0;										/* Byte Address */
    /* Data */
    sCommand.DataMode          		= HAL_OSPI_DATA_1_LINE;						/* Define Data Lines: Data On a Single Line */
    sCommand.DummyCycles       		= 0;										/* Bytes Send With No Data */
    sCommand.NbData            		= 1;										/* Bytes Send With Data */

	if (register_num == 1)
		sCommand.Instruction = W25Q_READ_SR1_CMD;
	else if (register_num == 2)
		sCommand.Instruction = W25Q_READ_SR2_CMD;
	else if (register_num == 3)
		sCommand.Instruction = W25Q_READ_SR3_CMD;
	else
		return HAL_ERROR;

    if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (HAL_OSPI_Receive(&hospi2, register_data, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return HAL_ERROR;
    }

	return HAL_OK;
}

/* Write Status Registers Function */
HAL_StatusTypeDef w25q128_write_status_registers(uint8_t reg_data, uint8_t reg_num)
{
	OSPI_RegularCmdTypeDef sCommand;

	/* Common Commands*/
    ospi_command_default_init(&sCommand);
    /* Instruction */
    sCommand.Instruction 			= W25Q_WRITE_DISABLE_CMD;					/* What We Do? */
    /* Address */
    sCommand.AddressMode       		= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
    sCommand.Address				= 0;										/* Byte Address */
    /* Data */
    sCommand.DataMode          		= HAL_OSPI_DATA_NONE;						/* Define Data Lines: No Data */
    sCommand.DummyCycles       		= 0;										/* Bytes Send With No Data */
    sCommand.NbData            		= 0;										/* Bytes Send With Data */

	if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	if (w25q128_auto_polling_mem_ready() != HAL_OK) {
        return HAL_ERROR;
    }

	sCommand.Instruction 			= W25Q_ENABLE_VOLATILE_SR_CMD;				/* What We Do? */
	if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	if (w25q128_auto_polling_mem_ready() != HAL_OK) {
        return HAL_ERROR;
    }

	if (reg_num == 1)
		sCommand.Instruction = W25Q_WRITE_SR1_CMD;
	else if (reg_num == 2)
		sCommand.Instruction = W25Q_WRITE_SR2_CMD;
	else if (reg_num == 3)
		sCommand.Instruction = W25Q_WRITE_SR3_CMD;
	else
		return HAL_ERROR;

	sCommand.DataMode          		= HAL_OSPI_DATA_1_LINE;
	sCommand.NbData            		= 1;

	if (w25q128_write_enable() != HAL_OK) {
		return HAL_ERROR;
	}

	if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	if (HAL_OSPI_Transmit(&hospi2, &reg_data, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	if (w25q128_auto_polling_mem_ready() != HAL_OK) {
        return HAL_ERROR;
    }

	return HAL_OK;
}






HAL_StatusTypeDef w25q128_test() {
    static const uint32_t sectors_count = W25Q_FLASH_SIZE / W25Q_SECTOR_SIZE;
    static const uint32_t memory_sector_size = W25Q_SECTOR_SIZE;

    uint8_t buffer_test[memory_sector_size];
    uint32_t var = 0;

    // Initialize OSPI
    if (w25q128_init() != HAL_OK) return HAL_ERROR;

    // Fill buffer with test data
    for (var = 0; var < memory_sector_size; var++) {
        buffer_test[var] = (var & 0xFF);
    }

    // Erase and write to each sector
    for (var = 0; var < sectors_count; var++) {
        MG_INFO(("Writing sector %lu to address 0x%08lX\r\n", var, var * memory_sector_size));

        if (w25q128_erase_sector(var * memory_sector_size,
                                     (var + 1) * memory_sector_size - 1) != HAL_OK) {
            MG_INFO(("Erase failed at sector %lu\r\n", var));
            return HAL_ERROR;
        }

        if (w25q128_write(buffer_test, var * memory_sector_size, memory_sector_size) != HAL_OK) {
            MG_INFO(("Write failed at sector %lu\r\n", var));
            return HAL_ERROR;
        }
    }

    // Enable memory-mapped mode
    if (w25q128_memmap_enable() != HAL_OK) {
        MG_INFO(("Failed to enable memory-mapped mode\r\n"));
        return HAL_ERROR;
    }

    // Validate written data via memory-mapped access
    for (var = 0; var < sectors_count; var++) {
        uint8_t *flash_ptr = (uint8_t*)(0x70000000 + var * memory_sector_size);

        MG_INFO(("\r\nChecking sector %lu at address 0x%08lX\r\n", var, (uint32_t)flash_ptr));
        printf("First 100 bytes:\r\n");
        for (int i = 0; i < 100; i++) {
            printf("%02X ", flash_ptr[i]);
            if ((i + 1) % 16 == 0) printf("\r\n");
        }

        if (memcmp(buffer_test, flash_ptr, memory_sector_size) != 0) {
            MG_INFO(("\r\nMismatch at sector %lu (address 0x%08lX)\r\n", var, (uint32_t)flash_ptr));
            return HAL_ERROR;
        }
    }

    MG_INFO(("\r\nQSPI memory check passed for %lu sectors.\r\n", sectors_count));
    return HAL_OK;
}



// QSPI_FLASH_SECTION void ext_flash_exe_test(void)
// {
//   MG_INFO(("External flash XIP works\r\n"));
// }