/**
 * @file hi2c1.c
 * @author Yam Goz
 * @brief in this file you should chose the I2C mode of your choice for handle #1:
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


#include "serial_comm/i2c/hi2c1.h"
#include "cmsis_os.h"  // for osDelay()


I2C_HandleTypeDef hi2c1;  // Actual definition (memory allocation)


// cannot change name for cube mx generated code
void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */
    // Select timing based on I2C mode
    #if I2C1_MODE == I2C_MODE_STD
        hi2c1.Init.Timing = 0x60404E72;  // 100kHz
    #elif I2C1_MODE == I2C_MODE_FAST
        hi2c1.Init.Timing = 0x00D049FB;  // 400kHz
    #elif I2C1_MODE == I2C_MODE_FAST_PLUS
        hi2c1.Init.Timing = 0x00601A5C;  // 1MHz
        HAL_I2CEx_EnableFastModePlus(I2C_FASTMODEPLUS_I2C1);
    #else
        #error "Invalid I2C1_MODE defined"
    #endif

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.OwnAddress1 = 0x50;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}


// Blocking calls
// Master transmit (raw data)
HAL_StatusTypeDef I2C1_master_transmit(I2C_packet data_packet, uint16_t slave_addr) {
  HAL_StatusTypeDef status = HAL_I2C_Master_Transmit(&hi2c1, slave_addr, data_packet.data, data_packet.size, 1000);
  osDelay(I2C1_OP_DELAY);  // Wait 50 ms after transmit (e.g., EEPROM internal write time)
  return status;
}

// Master receive (raw data)
HAL_StatusTypeDef I2C1_master_receive(I2C_packet buffer_packet, uint16_t slave_addr) {
  HAL_StatusTypeDef status = HAL_I2C_Master_Receive(&hi2c1, slave_addr, buffer_packet.data, buffer_packet.size, 1000);
  // Usually no delay needed after receive
  return status;
}

// Memory write (EEPROM-style: memory address + data)
HAL_StatusTypeDef I2C1_mem_write(uint16_t mem_address, I2C_packet data_packet, uint16_t slave_addr) {
  HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c1, slave_addr, mem_address, I2C_MEMADD_SIZE_16BIT,
                                              data_packet.data, data_packet.size, 1000);
  osDelay(I2C1_OP_DELAY);  // EEPROM write cycle delay
  return status;
}

// Memory read (EEPROM-style: memory address -> read into buffer)
HAL_StatusTypeDef I2C1_mem_read(uint16_t mem_address, I2C_packet buffer_packet, uint16_t slave_addr) {
  HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1, slave_addr, mem_address, I2C_MEMADD_SIZE_16BIT,
                                             buffer_packet.data, buffer_packet.size, 1000);
  // Usually no delay needed after read
  return status;
}



// non-blocking calls...

// Master transmit (interrupt-based)
HAL_StatusTypeDef I2C1_master_transmit_IT(I2C_packet data_packet, uint16_t slave_addr) {
    return HAL_I2C_Master_Transmit_IT(&hi2c1, slave_addr, data_packet.data, data_packet.size);
}

// Master receive (interrupt-based)
HAL_StatusTypeDef I2C1_master_receive_IT(I2C_packet buffer_packet, uint16_t slave_addr) {
    return HAL_I2C_Master_Receive_IT(&hi2c1, slave_addr, buffer_packet.data, buffer_packet.size);
}

// Slave transmit (interrupt-based)
HAL_StatusTypeDef I2C1_slave_transmit_IT(I2C_packet data_packet) {
    return HAL_I2C_Slave_Transmit_IT(&hi2c1, data_packet.data, data_packet.size);
}

// Slave receive (interrupt-based)
HAL_StatusTypeDef I2C1_slave_receive_IT(I2C_packet buffer_packet) {
    return HAL_I2C_Slave_Receive_IT(&hi2c1, buffer_packet.data, buffer_packet.size);
}