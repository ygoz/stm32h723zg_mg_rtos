/**
 * @file hi2c4.c
 * @author Yam Goz
 * @brief in this file you should chose the I2C mode of your choice for handle #4:
 * -  I2C_MODE_STD        100KHz
 * -  I2C_MODE_FAST       400KHz
 * -  I2C_MODE_FAST_PLUS  1MHz
 * 
 * ADDRESSING MODE IS SET TO 7 BITS!
 * @version 0.1
 * @date 2025-05-18
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#include "serial_comm/i2c/hi2c4.h"
#include "cmsis_os.h"  // for osDelay()


I2C_HandleTypeDef hi2c4;  // Actual definition (memory allocation)


// cannot change name for cube mx generated code
void MX_I2C4_Init(void)
{

  /* USER CODE BEGIN I2C4_Init 0 */

  /* USER CODE END I2C4_Init 0 */

  /* USER CODE BEGIN I2C4_Init 1 */
    // Select timing based on I2C mode
    #if I2C4_MODE == I2C_MODE_STD
        hi2c4.Init.Timing = 0x60404E72;  // 100kHz
    #elif I2C4_MODE == I2C_MODE_FAST
        hi2c4.Init.Timing = 0x00D049FB;  // 400kHz
    #elif I2C4_MODE == I2C_MODE_FAST_PLUS
        hi2c4.Init.Timing = 0x00601A5C;  // 1MHz
        HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C4);
    #else
        #error "Invalid I2C4_MODE defined"
    #endif

  /* USER CODE END I2C4_Init 1 */
  hi2c4.Instance = I2C4;
//   hi2c4.Init.Timing = 0x00D049FB;
  #ifdef HIL_SLAVE_MODE
  hi2c4.Init.OwnAddress1 = 0x50;
  #else
  hi2c4.Init.OwnAddress1 = 0;
  #endif
  hi2c4.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c4.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c4.Init.OwnAddress2 = 0;
  hi2c4.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c4.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c4.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c4, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c4, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C4_Init 2 */

  /* USER CODE END I2C4_Init 2 */

}


// Blocking calls
// Master transmit (raw data)
HAL_StatusTypeDef I2C4_master_transmit(I2C_packet data_packet, uint16_t slave_addr) {
  HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c4, slave_addr, data_packet.data, data_packet.size, 1000);
  osDelay(I2C4_OP_DELAY);  // Wait 50 ms after transmit (e.g., EEPROM internal write time)
  return status;
}

// Master receive (raw data)
HAL_StatusTypeDef I2C4_master_receive(I2C_packet buffer_packet, uint16_t slave_addr) {
  HAL_StatusTypeDef status = HAL_I2C_Master_Receive(&hi2c4, slave_addr, buffer_packet.data, buffer_packet.size, 1000);
  // Usually no delay needed after receive
  return status;
}

// Memory write (EEPROM-style: memory address + data)
HAL_StatusTypeDef I2C4_mem_write(uint16_t mem_address, I2C_packet data_packet, uint16_t slave_addr) {
  HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c4, slave_addr, mem_address, I2C_MEMADD_SIZE_16BIT,
                                              data_packet.data, data_packet.size, 1000);
  osDelay(I2C4_OP_DELAY);  // EEPROM write cycle delay
  return status;
}

// Memory read (EEPROM-style: memory address -> read into buffer)
HAL_StatusTypeDef I2C4_mem_read(uint16_t mem_address, I2C_packet buffer_packet, uint16_t slave_addr) {
  HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c4, slave_addr, mem_address, I2C_MEMADD_SIZE_16BIT,
                                             buffer_packet.data, buffer_packet.size, 1000);
  // Usually no delay needed after read
  return status;
}





