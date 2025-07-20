#include "serial_comm/spi/octospi.h"
#include "serial_comm/spi/octospi_spi.h"
#include "serial_comm/spi/utils.h"
#include "flash/w25q128jvsq.h"


/* Reset Chip Function */
HAL_StatusTypeDef W25Q128_SPI_ResetChip(OSPI_HandleTypeDef* hospi)
{
    OSPI_RegularCmdTypeDef sCommand={0};

    /* Enable Reset --------------------------- */
	/* Common Commands*/
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
	sCommand.FlashId            		= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
	sCommand.DataDtrMode				= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
	sCommand.SIOOMode          			= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
	sCommand.AlternateBytes				= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
	sCommand.AlternateBytesSize			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
	sCommand.AlternateBytesDtrMode		= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
	sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
	sCommand.AddressSize 				= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
	/* Instruction */
	sCommand.Instruction 				= W25Q_ENABLE_RST_CMD;						/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
	sCommand.Address					= 0;										/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_NONE;						/* Define Data Lines: No Data */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= 0;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    /* Reset Device --------------------------- */
	/* Common Commands*/
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
	sCommand.FlashId            		= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
	sCommand.DataDtrMode				= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
	sCommand.SIOOMode          			= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
	sCommand.AlternateBytes				= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
	sCommand.AlternateBytesSize			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
	sCommand.AlternateBytesDtrMode		= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
	sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
	sCommand.AddressSize 				= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
	/* Instruction */
	sCommand.Instruction 				= W25Q_RESET_CMD;							/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
	sCommand.Address					= 0;										/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_NONE;						/* Define Data Lines: No Data */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= 0;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }
    return HAL_OK;
}


/* Read Status Registers Function */
HAL_StatusTypeDef W25Q128_SPI_Read_Status_Registers(OSPI_HandleTypeDef* hospi, uint8_t* register_data, uint8_t register_num)
{
	OSPI_RegularCmdTypeDef sCommand={0};

	/* Common Commands*/
    sCommand.OperationType      	= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
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
    else if (register_num == 4)
		sCommand.Instruction = 0x9F;
	else
		return HAL_ERROR;

    if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return HAL_ERROR;
    }

    if (HAL_OSPI_Receive(hospi, register_data, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return HAL_ERROR;
    }

	return HAL_OK;
}

/* Write Status Registers Function */
HAL_StatusTypeDef W25Q128_SPI_Write_Status_Registers(OSPI_HandleTypeDef* hospi, uint8_t reg_data, uint8_t reg_num)
{
	OSPI_RegularCmdTypeDef sCommand;

	/* Common Commands*/
    sCommand.OperationType      	= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
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
    sCommand.Instruction 			= W25Q_WRITE_DISABLE_CMD;					/* What We Do? */
    /* Address */
    sCommand.AddressMode       		= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
    sCommand.Address				= 0;										/* Byte Address */
    /* Data */
    sCommand.DataMode          		= HAL_OSPI_DATA_NONE;						/* Define Data Lines: No Data */
    sCommand.DummyCycles       		= 0;										/* Bytes Send With No Data */
    sCommand.NbData            		= 0;										/* Bytes Send With Data */

	if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	if (W25Q128_OSPI_AutoPollingMemReady(hospi) != HAL_OK) {
        return HAL_ERROR;
    }

	sCommand.Instruction 			= W25Q_ENABLE_VOLATILE_SR_CMD;				/* What We Do? */
	if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	if (W25Q128_OSPI_AutoPollingMemReady(hospi) != HAL_OK) {
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

	if (W25Q128_OSPI_WriteEnable(hospi) != HAL_OK) {
		return HAL_ERROR;
	}

	if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	if (HAL_OSPI_Transmit(hospi, &reg_data, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
		return HAL_ERROR;
	}
	if (W25Q128_OSPI_AutoPollingMemReady(hospi) != HAL_OK) {
        return HAL_ERROR;
    }

	return HAL_OK;
}

HAL_StatusTypeDef W25Q128_SPI_Configuration(OSPI_HandleTypeDef* hospi)
{
    uint8_t reg1 = 0, reg2 = 0, reg3 = 0;
    uint8_t w_reg1 = 0, w_reg2 = 0, w_reg3 = 0;
    
    uint8_t jedec_id = 0;

    // Read current status registers
    if (W25Q128_SPI_Read_Status_Registers(hospi, &reg1, 1) != HAL_OK)
        return HAL_ERROR;
    if (W25Q128_SPI_Read_Status_Registers(hospi, &reg2, 2) != HAL_OK)
        return HAL_ERROR;
    if (W25Q128_SPI_Read_Status_Registers(hospi, &reg3, 3) != HAL_OK)
        return HAL_ERROR;
    if (W25Q128_SPI_Read_Status_Registers(hospi, &jedec_id, 4) != HAL_OK)
        return HAL_ERROR;

    printf("Status Registers BEFORE configuration:\r\n");
    printf("SR1: 0x%02X, SR2: 0x%02X, SR3: 0x%02X, jedec_id: 0x%02X\r\n", reg1, reg2, reg3, jedec_id);

    // Modify the status registers
    // w_reg1 = reg1;
    w_reg1 = 0;
    w_reg2 = 0;
    w_reg3 = (reg3 & W25Q_SR_DRV1);  // Masked write for SR3

    if (W25Q128_SPI_Write_Status_Registers(hospi, w_reg1, 1) != HAL_OK)
        return HAL_ERROR;
    if (W25Q128_SPI_Write_Status_Registers(hospi, w_reg2, 2) != HAL_OK)
        return HAL_ERROR;
    if (W25Q128_SPI_Write_Status_Registers(hospi, w_reg3, 3) != HAL_OK)
        return HAL_ERROR;

    // Read again after write
    if (W25Q128_SPI_Read_Status_Registers(hospi, &reg1, 1) != HAL_OK)
        return HAL_ERROR;
    if (W25Q128_SPI_Read_Status_Registers(hospi, &reg2, 2) != HAL_OK)
        return HAL_ERROR;
    if (W25Q128_SPI_Read_Status_Registers(hospi, &reg3, 3) != HAL_OK)
        return HAL_ERROR;

    printf("Status Registers AFTER configuration:\r\n");
    printf("SR1: 0x%02X, SR2: 0x%02X, SR3: 0x%02X\r\n", reg1, reg2, reg3);

    return HAL_OK;
}






/* Auto Polling Memory Function */
HAL_StatusTypeDef W25Q128_SPI_AutoPollingMemReady(OSPI_HandleTypeDef* hospi)
{

    OSPI_RegularCmdTypeDef sCommand;
    OSPI_AutoPollingTypeDef sConfig;

    /* Configure automatic polling mode to wait for memory ready ------ */
	/* Common Commands*/
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
	sCommand.FlashId            		= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
	sCommand.DataDtrMode				= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
	sCommand.SIOOMode          			= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
	sCommand.AlternateBytes				= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
	sCommand.AlternateBytesSize			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
	sCommand.AlternateBytesDtrMode		= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
	sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
	sCommand.AddressSize 				= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
	/* Instruction */
	sCommand.Instruction 				= W25Q_READ_SR1_CMD;						/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
	sCommand.Address					= 0;										/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_1_LINE;						/* Define Data Lines: Data On a Single Line */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= 1;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    sConfig.Match           			= 0x00U;
    sConfig.Mask            			= 0x01U;
    sConfig.MatchMode       			= HAL_OSPI_MATCH_MODE_AND;
    sConfig.Interval        			= W25Q_AUTOPOLLING_INTERVAL_TIME;
    sConfig.AutomaticStop   			= HAL_OSPI_AUTOMATIC_STOP_ENABLE;

    if (HAL_OSPI_AutoPolling(hospi, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}





/* Write Enable Function */
HAL_StatusTypeDef W25Q128_SPI_WriteEnable(OSPI_HandleTypeDef* hospi)
{
    OSPI_RegularCmdTypeDef sCommand;
    OSPI_AutoPollingTypeDef sConfig;

    /* Enable write operations ------------------------------------------ */
	/* Common Commands*/
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
	sCommand.FlashId            		= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
	sCommand.DataDtrMode				= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
	sCommand.SIOOMode          			= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
	sCommand.AlternateBytes				= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
	sCommand.AlternateBytesSize			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
	sCommand.AlternateBytesDtrMode		= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
	sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
	sCommand.AddressSize 				= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
	/* Instruction */
	sCommand.Instruction 				= W25Q_WRITE_ENABLE_CMD;					/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
	sCommand.Address					= 0;										/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_NONE;						/* Define Data Lines: No Data */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= 0;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

	/* Common Commands*/
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
	sCommand.FlashId            		= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
	sCommand.DataDtrMode				= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
	sCommand.SIOOMode          			= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
	sCommand.AlternateBytes				= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
	sCommand.AlternateBytesSize			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
	sCommand.AlternateBytesDtrMode		= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
	sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
	sCommand.AddressSize 				= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
	/* Instruction */
	sCommand.Instruction 				= W25Q_READ_SR1_CMD;						/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_NONE;					/* Define Address Lines: No Address */
	sCommand.Address					= 0;										/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_1_LINE;						/* Define Data Lines: Data On a Single Line */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= 1;										/* Bytes Send With Data */

    if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    /* Configure automatic polling mode to wait for write enabling ---- */
    sConfig.Match 					= 0x02U;
    sConfig.Mask 					= 0x02U;
    sConfig.MatchMode 				= HAL_OSPI_MATCH_MODE_AND;
    sConfig.Interval 				= W25Q_AUTOPOLLING_INTERVAL_TIME;
    sConfig.AutomaticStop 			= HAL_OSPI_AUTOMATIC_STOP_ENABLE;


    if (HAL_OSPI_AutoPolling(hospi, &sConfig, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
        return HAL_ERROR;
    }

    return HAL_OK;
}






HAL_StatusTypeDef W25Q128_SPI_Init(OSPI_HandleTypeDef* hospi)
{
	if (HAL_OSPI_DeInit(hospi) != HAL_OK) {
	    return HAL_ERROR;
	}

	MX_OCTOSPI2_Init();

	if (W25Q128_SPI_ResetChip(hospi) != HAL_OK) {
	    return HAL_ERROR;
	}
	if (W25Q128_SPI_Configuration(hospi) != HAL_OK) {
        return HAL_ERROR;
    }
	HAL_Delay(1);
	if (W25Q128_SPI_AutoPollingMemReady(hospi) != HAL_OK) {
        return HAL_ERROR;
    }
    if (W25Q128_SPI_WriteEnable(hospi) != HAL_OK) {
        return HAL_ERROR;
    }

    uint8_t reg1 = 0;
    if (W25Q128_SPI_Read_Status_Registers(hospi, &reg1, 1) != HAL_OK)
        return HAL_ERROR;

    printf("Status Registers BEFORE configuration:\r\n");
    printf("SR1: 0x%02X\r\n", reg1);

    return HAL_OK;
}







/* Erase Sector Function */
HAL_StatusTypeDef W25Q128_SPI_EraseSector(OSPI_HandleTypeDef* hospi, uint32_t EraseStartAddress, uint32_t EraseEndAddress)
{
    OSPI_RegularCmdTypeDef sCommand={0};
    uint32_t StartAddress=0;

    StartAddress = EraseStartAddress - (EraseStartAddress % W25Q_SECTOR_SIZE);

    while (EraseEndAddress >= StartAddress)
    {
    	/* Erasing Sequence -------------------------------------------------- */
    	/* Common Commands*/
    	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
    	sCommand.FlashId            		= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
    	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
    	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
    	sCommand.DataDtrMode				= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
    	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
    	sCommand.SIOOMode          			= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
    	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
    	sCommand.AlternateBytes				= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
    	sCommand.AlternateBytesSize			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
    	sCommand.AlternateBytesDtrMode		= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
    	sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
    	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
    	sCommand.AddressSize 				= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
    	/* Instruction */
    	sCommand.Instruction 				= W25Q_SECTOR_ERASE_CMD;				/* What We Do? */
    	/* Address */
    	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_1_LINE;					/* Define Address Lines: Address On a Single Line */
    	sCommand.Address					= (StartAddress & 0xFFFFFF);				/* Byte Address */
    	/* Data */
    	sCommand.DataMode          			= HAL_OSPI_DATA_NONE;						/* Define Data Lines: No Data */
    	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
    	sCommand.NbData            			= 0;										/* Bytes Send With Data */

        if (W25Q128_SPI_WriteEnable(hospi) != HAL_OK) {
            return HAL_ERROR;
        }

        if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK) {
            return HAL_ERROR;
        }

        if (W25Q128_SPI_AutoPollingMemReady(hospi) != HAL_OK) {
            return HAL_ERROR;
        }

        StartAddress += W25Q_SECTOR_SIZE;
    }

    return HAL_OK;
}

/* Write Function */
HAL_StatusTypeDef W25Q128_SPI_Write(OSPI_HandleTypeDef* hospi, uint8_t* pData, uint32_t WriteAddr, uint32_t Size)
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
	sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
	sCommand.FlashId            		= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
	sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
	sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
	sCommand.DataDtrMode				= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
	sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
	sCommand.SIOOMode          			= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
	sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
	sCommand.AlternateBytes				= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
	sCommand.AlternateBytesSize			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
	sCommand.AlternateBytesDtrMode		= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
	sCommand.InstructionMode   			= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
	sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
	sCommand.AddressSize 				= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
	/* Instruction */
	sCommand.Instruction 				= CMD_PAGE_PROGRAM;			/* What We Do? */
	/* Address */
	sCommand.AddressMode       			= HAL_OSPI_ADDRESS_1_LINE;					/* Define Address Lines: Address On a Single Line */
	sCommand.Address					= current_addr;								/* Byte Address */
	/* Data */
	sCommand.DataMode          			= HAL_OSPI_DATA_1_LINE;					/* Define Data Lines: Data On Four Lines */
	sCommand.DummyCycles       			= 0;										/* Bytes Send With No Data */
	sCommand.NbData            			= current_size;								/* Bytes Send With Data */

    if (current_size == 0) {
        return HAL_OK;
    }

    /* Enable write operations */
    if (W25Q128_OSPI_WriteEnable(hospi) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Configure the command */
    if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Transmission of the data */
    if (HAL_OSPI_Transmit(hospi, (uint8_t*)data_addr, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
      return HAL_ERROR;
    }

    /* Configure automatic polling mode to wait for end of program */
    if (W25Q128_OSPI_AutoPollingMemReady(hospi) != HAL_OK)
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
HAL_StatusTypeDef W25Q128_SPI_Read(OSPI_HandleTypeDef* hospi,uint8_t* pData, uint32_t ReadAddr, uint32_t Size)
{
  OSPI_RegularCmdTypeDef sCommand={0};
  /* Initialize the read command */
  /* Common Commands*/
  sCommand.OperationType      		= HAL_OSPI_OPTYPE_COMMON_CFG; 				/* Common configuration (indirect or auto-polling mode) */
  sCommand.FlashId            		= HAL_OSPI_FLASH_ID_1; 						/* Set The OCTO SPI Flash ID */
  sCommand.InstructionDtrMode 		= HAL_OSPI_INSTRUCTION_DTR_DISABLE; 		/* Disable Instruction DDR/DTR Mode */
  sCommand.AddressDtrMode     		= HAL_OSPI_ADDRESS_DTR_DISABLE; 			/* Disable Address DDR/DTR Mode */
  sCommand.DataDtrMode				= HAL_OSPI_DATA_DTR_DISABLE; 				/* Disable Data DDR/DTR Mode */
  sCommand.DQSMode            		= HAL_OSPI_DQS_DISABLE; 					/* Disable Data Strobe */
  sCommand.SIOOMode          		= HAL_OSPI_SIOO_INST_EVERY_CMD; 			/* SIOO Mode: Send instruction on every transaction */
  sCommand.AlternateBytesMode 		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Disable Alternate Bytes Mode */
  sCommand.AlternateBytes			= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes = 0 */
  sCommand.AlternateBytesSize		= HAL_OSPI_ALTERNATE_BYTES_NONE; 			/* Alternate Bytes Size = 0 */
  sCommand.AlternateBytesDtrMode	= HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE; 	/* Disable Alternate Bytes DDR/DTR Mode */
  sCommand.InstructionMode   		= HAL_OSPI_INSTRUCTION_1_LINE;				/* Instruction on a single line */
  sCommand.InstructionSize    		= HAL_OSPI_INSTRUCTION_8_BITS;				/* 8-bit Instruction */
  sCommand.AddressSize 				= HAL_OSPI_ADDRESS_24_BITS;					/* 24-bit Address */
  /* Instruction */
  sCommand.Instruction 				= CMD_READ_DATA;				/* What We Do? */
  /* Address */
  sCommand.AddressMode       		= HAL_OSPI_ADDRESS_1_LINE;					/* Define Address Lines: Address On Four Line */
  sCommand.Address					= ReadAddr;									/* Byte Address */
  /* Data */
  sCommand.DataMode          		= HAL_OSPI_DATA_1_LINE;					/* Define Data Lines: Data On Four Lines */
  sCommand.DummyCycles       		= 0;										/* Bytes Send With No Data */
  sCommand.NbData            		= Size;										/* Bytes Send With Data */

  /* Configure the command */
  if (HAL_OSPI_Command(hospi, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  /* Reception of the data */
  if (HAL_OSPI_Receive(hospi, pData, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return HAL_ERROR;
  }

  return HAL_OK;
}




















/**
 * @brief Send Write Enable command (0x06).
 * @return HAL_OK on success, HAL_ERROR on failure.
 */
// HAL_StatusTypeDef w25q_write_enable(OSPI_HandleTypeDef *hospi) {
//     uint8_t cmd = CMD_WRITE_ENABLE;
//     HAL_StatusTypeDef status = W25Q128_OSPI_WriteEnable(hospi);
//     if (status != HAL_OK) {
//         LOG_INFO("Write Enable failed: %d\r\n", status);
//     }
//     return status;
// }





// /**
//  * @brief Wait until Flash is not busy (WIP bit = 0).
//  * @return HAL_OK on success, HAL_TIMEOUT or HAL_ERROR on failure.
//  */
// HAL_StatusTypeDef w25q_wait_busy(SPI_HandleTypeDef *hspi) {
//     uint8_t cmd[2] = {CMD_READ_STATUS, 0xFF};
//     uint8_t rx[2] = {0};
//     uint32_t start = HAL_GetTick();

//     do {
//         HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hspi, cmd, rx, 2, SPI_TIMEOUT);
//         if (status != HAL_OK) {
//             LOG_INFO("Read Status failed: %d\r\n", status);
//             return status;
//         }
//         if ((HAL_GetTick() - start) > 1000) {
//             LOG_INFO("Wait Busy timeout\r\n");
//             return HAL_TIMEOUT;
//         }
//     } while (rx[1] & 0x01);

//     return HAL_OK;
// }

// /**
//  * @brief Erase a sector (4 KB) at specified address.
//  * @param hspi Pointer to SPI handle.
//  * @param addr 24-bit sector address (must be sector-aligned).
//  * @return HAL_OK on success, HAL_ERROR on failure.
//  */
// HAL_StatusTypeDef w25q_erase_sector(SPI_HandleTypeDef *hspi, uint32_t addr) {
//     addr &= W25Q_MAX_ADDR;  // Mask to 24 bits
//     uint8_t cmd[4] = {CMD_SECTOR_ERASE,
//                       (uint8_t)(addr >> 16),
//                       (uint8_t)(addr >> 8),
//                       (uint8_t)addr};

//     HAL_StatusTypeDef status = w25q_write_enable(hspi);
//     if (status != HAL_OK) {
//         return status;
//     }
//     status = HAL_SPI_Transmit(hspi, cmd, 4, SPI_TIMEOUT);
//     if (status != HAL_OK) {
//         LOG_INFO("Sector Erase failed at 0x%06lx: %d\r\n", addr, status);
//         return status;
//     }
//     return w25q_wait_busy(hspi);
// }

// /**
//  * @brief Write data to a page (256 bytes max) at specified address.
//  * @param hspi Pointer to SPI handle.
//  * @param addr 24-bit page start address (must be page-aligned).
//  * @param data Pointer to data to write.
//  * @param len Length of data (max 256 bytes).
//  * @return HAL_OK on success, HAL_ERROR on failure.
//  */
// HAL_StatusTypeDef w25q_write_page(SPI_HandleTypeDef *hspi, uint32_t addr, uint8_t *data, uint16_t len) {
//     if (data == NULL || len > 256) {
//         LOG_INFO("Invalid write parameters: data=%p, len=%u\r\n", data, len);
//         return HAL_ERROR;
//     }
//     addr &= W25Q_MAX_ADDR;  // Mask to 24 bits

//     uint8_t cmd[4 + 256] = {0};
//     cmd[0] = CMD_PAGE_PROGRAM;
//     cmd[1] = (uint8_t)(addr >> 16);
//     cmd[2] = (uint8_t)(addr >> 8);
//     cmd[3] = (uint8_t)addr;
//     memcpy(&cmd[4], data, len);

//     HAL_StatusTypeDef status = w25q_write_enable(hspi);
//     if (status != HAL_OK) {
//         return status;
//     }
//     status = HAL_SPI_Transmit(hspi, cmd, 4 + len, SPI_TIMEOUT);
//     if (status != HAL_OK) {
//         LOG_INFO("Page Program failed at 0x%06lx: %d\r\n", addr, status);
//         return status;
//     }
//     return w25q_wait_busy(hspi);
// }

// /**
//  * @brief Read data from specified 24-bit address in W25Q128JV.
//  * @param hspi Pointer to SPI handle.
//  * @param addr 32-bit address (only lower 24 bits used, 0x000000 to 0xFFFFFF).
//  * @param rx Pointer to buffer to store read data.
//  * @param len Length of data to read (max 65535 bytes).
//  * @return HAL_OK on success, HAL_ERROR on invalid parameters or SPI failure.
//  */
// HAL_StatusTypeDef w25q_read(SPI_HandleTypeDef *hspi, uint32_t addr, uint8_t *rx, uint16_t len) {
//     if (rx == NULL || len == 0 || len > W25Q_MAX_READ_LEN || (addr + len - 1) > W25Q_MAX_ADDR) {
//         LOG_INFO("Invalid read parameters: addr=0x%lx, rx=%p, len=%u\r\n", addr, rx, len);
//         return HAL_ERROR;
//     }

//     addr &= W25Q_MAX_ADDR;

//     // Allocate temporary buffer on heap
//     uint8_t *rx_buf = (uint8_t *)malloc(4 + len);
//     if (rx_buf == NULL) {
//         LOG_INFO("Memory allocation failed for %u bytes\r\n", 4 + len);
//         return HAL_ERROR;
//     }
//     memset(rx_buf, 0, 4 + len);

//     // Prepare command
//     rx_buf[0] = CMD_READ_DATA;
//     rx_buf[1] = (uint8_t)(addr >> 16);
//     rx_buf[2] = (uint8_t)(addr >> 8);
//     rx_buf[3] = (uint8_t)addr;

//     HAL_StatusTypeDef status = HAL_SPI_TransmitReceive(hspi, rx_buf, rx_buf, 4 + len, SPI_TIMEOUT);
//     if (status != HAL_OK) {
//         LOG_INFO("Read Data failed: %d\r\n", status);
//         free(rx_buf);
//         return status;
//     }

//     memcpy(rx, &rx_buf[4], len);
//     free(rx_buf);
//     return HAL_OK;
// }


// HAL_StatusTypeDef flash_erase_sector(SPI_Flash *self, uint32_t addr) {
//     return w25q_erase_sector(self->hspi, addr);
// }

// HAL_StatusTypeDef flash_write_page(SPI_Flash *self, uint32_t addr, uint8_t *data, uint16_t len) {
//     return w25q_write_page(self->hspi, addr, data, len);
// }

// HAL_StatusTypeDef flash_read(SPI_Flash *self, uint32_t addr, uint8_t *rx, uint16_t len) {
//     return w25q_read(self->hspi, addr, rx, len);
// }