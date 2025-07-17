/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "string.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "mongoose.h"
#include "stm32h7xx_hal.h"
#include "http/routers/main_router.h"
#include "http/settings/network.h"
#include "serial_comm/i2c/hi2c4.h"
#include "peripherals/adc/hadc3.h"
#include "peripherals/adc/hadc2.h"
#include "peripherals/adc/utils.h"
#include "peripherals/dac/hdac1.h"
#include "peripherals/dts/hdts.h"
#include "peripherals/comp/hcomp1.h"
#include "serial_comm/uart/huart10.h"
#include "serial_comm/uart/huart8.h"
#include "peripherals/timer/htim8.h"
#include "serial_comm/spi/hspi4.h"
#include "serial_comm/spi/hspi5.h"
#include "serial_comm/spi/octospi.h"
#include "flash/w25q128jvsq.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
#pragma location=0x30000000
ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
#pragma location=0x30000080
ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

#elif defined ( __CC_ARM )  /* MDK ARM Compiler */

__attribute__((at(0x30000000))) ETH_DMADescTypeDef  DMARxDscrTab[ETH_RX_DESC_CNT]; /* Ethernet Rx DMA Descriptors */
__attribute__((at(0x30000080))) ETH_DMADescTypeDef  DMATxDscrTab[ETH_TX_DESC_CNT]; /* Ethernet Tx DMA Descriptors */

#elif defined ( __GNUC__ ) /* GNU Compiler */

ETH_DMADescTypeDef DMARxDscrTab[ETH_RX_DESC_CNT] __attribute__((section(".RxDescripSection"))); /* Ethernet Rx DMA Descriptors */
ETH_DMADescTypeDef DMATxDscrTab[ETH_TX_DESC_CNT] __attribute__((section(".TxDescripSection")));   /* Ethernet Tx DMA Descriptors */
#endif

ETH_TxPacketConfig TxConfig;


ETH_HandleTypeDef heth;

RNG_HandleTypeDef hrng;

UART_HandleTypeDef huart3;


/* Definitions for Server */
osThreadId_t ServerHandle;
const osThreadAttr_t Server_attributes = {
  .name = "Server",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);
static void MX_RNG_Init(void);
void server(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//  try to move to syscalls.c 
// int _write(int fd, unsigned char *buf, int len) {
//   if (fd == 1 || fd == 2) {                     // stdout or stderr ?
//     HAL_UART_Transmit(&huart3, buf, len, 999);  // Print to the UART
//   }
//   return len;
// }


// In RTOS environment, run this function in a separate task. Give it 8k stack
static void run_mongoose(void) {
  struct mg_mgr mgr = {0};        // Mongoose event manager
  mg_mgr_init(&mgr);        // Initialise event manager


  mg_http_listen(&mgr, "http://0.0.0.0:80", event_handler, "");
  set_ip_configurations(&mgr);


  mg_log_set(MG_LL_DEBUG);  // Set log level to debug
//  HAL_GPIO_WritePin(GPIOB, LED_GREEN_Pin, GPIO_PIN_SET);
  // HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
  // HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);

  for (;;) {                // Infinite event loop
    mg_mgr_poll(&mgr, 10);   // Process network events
  }
}


#include "stm32h7xx_hal.h"  // or your MCU-specific HAL
#include <stm32h7xx_hal_ospi.h>

// extern OSPI_HandleTypeDef hospi2;  // Your OCTOSPI/QSPI handle

// uint8_t W25Q_ReadStatusRegister1(void)
// {
//     OSPI_RegularCmdTypeDef sCommand = {0};
//     uint8_t reg = 0;

//     // 0x05 = Read Status Register-1
//     sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
//     sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;
//     sCommand.Instruction        = 0x05; // input the reg definition
//     sCommand.InstructionMode    = HAL_OSPI_INSTRUCTION_1_LINE;
//     sCommand.AddressMode        = HAL_OSPI_ADDRESS_NONE;
//     sCommand.DataMode           = HAL_OSPI_DATA_1_LINE;
//     sCommand.DummyCycles        = 0;
//     sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
//     sCommand.NbData             = 1;

//     // Send the command
//     if (HAL_OSPI_Command(&hospi2, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
//         return 0xFF;

//     // Receive the data
//     if (HAL_OSPI_Receive(&hospi2, &reg, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
//         return 0xFF;

//     return reg;
// }

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
  SCB_EnableICache();
  // SCB_EnableDCache();

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_ETH_Init();
  MX_RNG_Init();
  /* USER CODE BEGIN 2 */
  // MX_OCTOSPI2_Init();

  //init all adcs here + calibration
  adc_init_all_handles();

  //init comp
  uint32_t comp_output;
  comp1_get_value(&comp_output);

  //init uart
  uart10.init(&uart10);
  uart8.init(&uart8);

  // **************************SPI***************************************
  // uint8_t *spi_rx;
  // uint8_t spi_tx[10] = "shalom aa";

  // const uint32_t test_addr = 0x000000;  // Must be sector-aligned (multiple of 4096)
  //   const char *test_data = "Hello from shalom Flash!";
  //   uint8_t read_buf[64] = {0};

  //   // 1. Erase sector
  //   if (my_flash.erase_sector(&my_flash, test_addr) != HAL_OK) {
  //       printf("Flash erase failed!\r\n");
  //       return;
  //   }

  //   // 2. Write string to flash
  //   if (my_flash.write_page(&my_flash, test_addr, (uint8_t *)test_data, strlen(test_data)) != HAL_OK) {
  //       printf("Flash write failed!\r\n");
  //       return;
  //   }

  //   // 3. Read back the string
  //   if (my_flash.read(&my_flash, test_addr, read_buf, strlen(test_data)) != HAL_OK) {
  //       printf("Flash read failed!\r\n");
  //       return;
  //   }

  //   // 4. Print result
  //   printf("Flash read: %s\r\n", read_buf);

// **************************************************QSPI*********************************************
// uint8_t write_data[] = "Hello";
// uint8_t *actual_write = write_data;
//     uint8_t read_data[sizeof(write_data)] = {0};
//     uint32_t address = 0x000000;

//     // Init + Reset + Config
//     W25Q128_OCTO_SPI_Init(&hospi2);

//     // Erase sector
//     W25Q128_OSPI_Erase_Chip(&hospi2);
//     if (W25Q128_OSPI_EraseSector(&hospi2, address, address + 4095) != HAL_OK) return;

//   // W25Q128_OSPI_WriteEnable(&hospi2);

// //     uint8_t status_reg = W25Q_ReadStatusRegister1();
// // printf("status_reg: 0x%02X\n", status_reg);

// // Check WEL (bit 1) and BUSY (bit 0)
// // if (status_reg & 0x01) printf("BUSY: Write/Erase in progress\n");
// // if (status_reg & 0x02) printf("WEL: Write Enable Latch is set\n");

//     // Write
//     if (W25Q128_OSPI_Write(&hospi2, write_data, address, sizeof(write_data)) != HAL_OK) return;


// // status_reg = W25Q_ReadStatusRegister1();
// // printf("status_reg: 0x%02X\n", status_reg);

// // Check WEL (bit 1) and BUSY (bit 0)
// // if (status_reg & 0x01) printf("BUSY: Write/Erase in progress\n");
// // if (status_reg & 0x02) printf("WEL: Write Enable Latch is set\n");


//     // Read
//     if (W25Q128_OSPI_Read(&hospi2, read_data, address, sizeof(read_data)) != HAL_OK) return;

//     // Output
//     printf("Read: %s\r\n", read_data);
// // Read data
//     if (W25Q128_OSPI_EnableMemoryMappedMode(&hospi2) != HAL_OK) return;

//     volatile uint8_t *ptr = (uint8_t *)0x70000000;
//     printf("First 100 bytes at 0x70000000 (hex):\r\n");
//     for (int i = 0; i < 100; i++) {
  //         printf("%02X ", ptr[i]);
  //         if ((i + 1) % 16 == 0) printf("\r\n");
  //     }
// **************************************************QSPI*********************************************

  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Server */
  ServerHandle = osThreadNew(server, NULL, &Server_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1) {
    printf("Tick: %lu\r\n", HAL_GetTick());
    HAL_Delay(500);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = 64;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 34;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_3;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 3072;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ETH Initialization Function
  * @param None
  * @retval None
  */
static void MX_ETH_Init(void)
{

  /* USER CODE BEGIN ETH_Init 0 */

  /* USER CODE END ETH_Init 0 */

   static uint8_t MACAddr[6];

  /* USER CODE BEGIN ETH_Init 1 */

  /* USER CODE END ETH_Init 1 */
  heth.Instance = ETH;
  MACAddr[0] = 0x00;
  MACAddr[1] = 0x80;
  MACAddr[2] = 0xE1;
  MACAddr[3] = 0x00;
  MACAddr[4] = 0x00;
  MACAddr[5] = 0x00;
  heth.Init.MACAddr = &MACAddr[0];
  heth.Init.MediaInterface = HAL_ETH_RMII_MODE;
  heth.Init.TxDesc = DMATxDscrTab;
  heth.Init.RxDesc = DMARxDscrTab;
  heth.Init.RxBuffLen = 1524;

  /* USER CODE BEGIN MACADDRESS */

  /* USER CODE END MACADDRESS */

  if (HAL_ETH_Init(&heth) != HAL_OK)
  {
    Error_Handler();
  }

  memset(&TxConfig, 0 , sizeof(ETH_TxPacketConfig));
  TxConfig.Attributes = ETH_TX_PACKETS_FEATURES_CSUM | ETH_TX_PACKETS_FEATURES_CRCPAD;
  TxConfig.ChecksumCtrl = ETH_CHECKSUM_IPHDR_PAYLOAD_INSERT_PHDR_CALC;
  TxConfig.CRCPadCtrl = ETH_CRC_PAD_INSERT;
  /* USER CODE BEGIN ETH_Init 2 */

  /* USER CODE END ETH_Init 2 */

}

/**
  * @brief RNG Initialization Function
  * @param None
  * @retval None
  */
static void MX_RNG_Init(void)
{

  /* USER CODE BEGIN RNG_Init 0 */

  /* USER CODE END RNG_Init 0 */

  /* USER CODE BEGIN RNG_Init 1 */

  /* USER CODE END RNG_Init 1 */
  hrng.Instance = RNG;
  hrng.Init.ClockErrorDetection = RNG_CED_ENABLE;
  if (HAL_RNG_Init(&hrng) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RNG_Init 2 */

  /* USER CODE END RNG_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pins : PE2 PE3 PE4 PE5
                           PE6 PE7 PE8 PE9
                           PE10 PE11 PE12 PE13
                           PE14 PE15 PE0 PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9
                          |GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13
                          |GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0|GPIO_PIN_1;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;      // Output Push-Pull
GPIO_InitStruct.Pull = GPIO_NOPULL;              // No internal pull-up/down
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;     // Low speed is fine for unused
HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

// Set output level to LOW
HAL_GPIO_WritePin(GPIOE, GPIO_InitStruct.Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 PC15 PC0
                           PC2 PC3 PC6 PC7
                           PC8 PC9 PC10 PC11
                           PC12 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15|GPIO_PIN_0
                          |GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;      // Output Push-Pull
GPIO_InitStruct.Pull = GPIO_NOPULL;              // No internal pull-up/down
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;     // Low speed is fine for unused
HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

// Set output level to LOW
HAL_GPIO_WritePin(GPIOC, GPIO_InitStruct.Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PF0 PF1 PF2 PF3
                           PF4 PF5 PF6 PF7
                           PF8 PF9 PF10 PF11
                           PF12 PF13 PF14 PF15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;      // Output Push-Pull
GPIO_InitStruct.Pull = GPIO_NOPULL;              // No internal pull-up/down
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;     // Low speed is fine for unused
HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

// Set output level to LOW
HAL_GPIO_WritePin(GPIOF, GPIO_InitStruct.Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PH0 PH1 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;      // Output Push-Pull
GPIO_InitStruct.Pull = GPIO_NOPULL;              // No internal pull-up/down
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;     // Low speed is fine for unused
HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

// Set output level to LOW
HAL_GPIO_WritePin(GPIOH, GPIO_InitStruct.Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PA0 PA3 PA4 PA5
                           PA6 PA8 PA9 PA10
                           PA11 PA12 PA13 PA14
                           PA15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5
                          |GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |GPIO_PIN_15;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;      // Output Push-Pull
GPIO_InitStruct.Pull = GPIO_NOPULL;              // No internal pull-up/down
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;     // Low speed is fine for unused
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

// Set output level to LOW
HAL_GPIO_WritePin(GPIOA, GPIO_InitStruct.Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PB0 PB1 PB2 PB10
                           PB11 PB12 PB14 PB15
                           PB3 PB4 PB5 PB6
                           PB7 PB8 PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_10
                          |GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6
                          |GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;      // Output Push-Pull
GPIO_InitStruct.Pull = GPIO_NOPULL;              // No internal pull-up/down
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;     // Low speed is fine for unused
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

// Set output level to LOW
HAL_GPIO_WritePin(GPIOB, GPIO_InitStruct.Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PG0 PG1 PG2 PG3
                           PG4 PG5 PG6 PG7
                           PG8 PG9 PG10 PG12
                           PG14 PG15 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7
                          |GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_12
                          |GPIO_PIN_14|GPIO_PIN_15;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;      // Output Push-Pull
GPIO_InitStruct.Pull = GPIO_NOPULL;              // No internal pull-up/down
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;     // Low speed is fine for unused
HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

// Set output level to LOW
HAL_GPIO_WritePin(GPIOG, GPIO_InitStruct.Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : PD8 PD9 PD10 PD11
                           PD12 PD13 PD14 PD15
                           PD0 PD1 PD2 PD3
                           PD4 PD5 PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11
                          |GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15
                          |GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;      // Output Push-Pull
GPIO_InitStruct.Pull = GPIO_NOPULL;              // No internal pull-up/down
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;     // Low speed is fine for unused
HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

// Set output level to LOW
HAL_GPIO_WritePin(GPIOD, GPIO_InitStruct.Pin, GPIO_PIN_RESET);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_server */
/**
  * @brief  Function implementing the Server thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_server */
void server(void *argument)
{
  /* USER CODE BEGIN 5 */
  NVIC_EnableIRQ(ETH_IRQn);   // preferably do this in Cube, as the tutorials above instruct
  run_mongoose();
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END 5 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
