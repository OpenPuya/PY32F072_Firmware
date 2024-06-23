/**
  ******************************************************************************
  * @file    main.c
  * @author  MCU Application Team
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 Puya Semiconductor Co.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by Puya under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
CAN_HandleTypeDef CanHandle;
CAN_FilterTypeDef CanFilter;

CAN_TxHeaderTypeDef CanTxHeader;
uint8_t TxData[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8};

CAN_RxHeaderTypeDef CanRxHeader;
uint8_t RxData[8] = {0};

uint8_t DLCtoBytes[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};

/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);

/**
  * @brief  Main program.
  * @retval int
  */
int main(void)
{
  uint32_t i = 0;
  
  /* Reset of all peripherals, Initializes the Systick. */
  HAL_Init();
  
  /* System clock configuration */
  APP_SystemClockConfig(); 
  
  /* Initialize debugging serial port (used by printf) */
  BSP_USART_Config();
  
  /* Initialize CAN */
  CanHandle.Instance                      = CAN1;
  CanHandle.Init.FrameFormat              = CAN_FRAME_CLASSIC;
  CanHandle.Init.Mode                     = CAN_MODE_LOOPBACK_INT; /* Internal loop mode */
  CanHandle.Init.Prescaler                = 6U; 
  CanHandle.Init.NominalSyncJumpWidth     = 4U;/* 250KHz */
  CanHandle.Init.NominalTimeSeg1          = 12U;
  CanHandle.Init.NominalTimeSeg2          = 4U;
  if (HAL_CAN_Init(&CanHandle) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  
  /* Configure CAN filters */
  CanFilter.IdType         = CAN_EXTENDED_ID;
  CanFilter.FilterChannel  = CAN_FILTER_CHANNEL_0;
  CanFilter.Rank           = CAN_FILTER_RANK_CHANNEL_NUMBER;
  CanFilter.FilterID       = 0x12345678; /* Consistent with sending message ID */
  CanFilter.FilterFormat   = 0xFFFFFFFF;
  CanFilter.MaskID         = 0x0; /* When receiving, only the 29 digits of the ID are compared */
  CanFilter.MaskFormat     = 0xFFFFFFFF;
  if (HAL_CAN_ConfigFilter(&CanHandle, &CanFilter) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  
  /* Start CAN */
  if (HAL_CAN_Start(&CanHandle) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  
  /* Add message to PTB */
  CanTxHeader.Identifier   = 0x12345678; 
  CanTxHeader.IdType       = CAN_EXTENDED_ID;
  CanTxHeader.TxFrameType  = CAN_DATA_FRAME;
  CanTxHeader.FrameFormat  = CAN_FRAME_CLASSIC;
  CanTxHeader.Handle       = 0x0;
  CanTxHeader.DataLength   = CAN_DLC_BYTES_8;
  if (HAL_CAN_AddMessageToTxFifo(&CanHandle, &CanTxHeader, TxData, CAN_TX_FIFO_PTB) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  
  /* Enable PTB transmission */
  if (HAL_CAN_ActivateTxRequest(&CanHandle, CAN_TXFIFO_PTB_SEND) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  
  /* Waiting for reception to complete */
  while (__HAL_CAN_GET_RX_FIFO_FILL_LEVEL(&CanHandle) == CAN_RX_FIFO_EMPTY)
  {
  }
  
  /* Read received data */
  if (HAL_CAN_GetRxMessage(&CanHandle, &CanRxHeader, RxData) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  
  /* Print received data */
  printf("decimal data length: %d\r\n", DLCtoBytes[CanRxHeader.DataLength]);
  printf("hexadecimal data: \r\n");
  for (i = 0; i < DLCtoBytes[CanRxHeader.DataLength]; i++)
  {
    printf("%x ", RxData[i]);
  }
  
  /* infinite loop */
  while (1)
  {
    
  }
}

/**
  * @brief  System clock configuration function.
  * @param  None
  * @retval None
  */
static void APP_SystemClockConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* Oscillator configuration */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE; /* Select oscillator HSE, HSI, LSI, LSE */
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                          /* Enable HSI */
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                          /* HSI 1 frequency division */
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_8MHz;  /* Configure HSI clock at 8MHz */
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;                          /* Enable HSE */
  RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz;
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;                         /* Close LSI */
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;                         /* Close LSE */
  /*RCC_OscInitStruct.LSEDriver = RCC_LSEDRIVE_MEDIUM;*/
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;                     /* Close PLL */
  /*RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_NONE;*/
  /*RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;*/
  /* Configure oscillator */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    APP_ErrorHandler();
  }

  /* Clock source configuration */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; /* Choose to configure clock HCLK, SYSCLK, PCLK1 */
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE; /* Select HSE as the system clock */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;     /* AHB clock 1 division */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;      /* APB clock 1 division */
  /* Configure clock source */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    APP_ErrorHandler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void APP_ErrorHandler(void)
{
  while (1)
  {
  }
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
