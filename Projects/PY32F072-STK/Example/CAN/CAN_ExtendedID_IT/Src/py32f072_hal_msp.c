/**
  ******************************************************************************
  * @file    py32f072_hal_msp.c
  * @author  MCU Application Team
  * @brief   This file provides code for the MSP Initialization
  *          and de-Initialization codes.
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

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* External functions --------------------------------------------------------*/

/**
  * @brief Initialize global MSP.
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

/**
  * @brief Initialize CAN MSP
  */
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
  RCC_OscInitTypeDef  OscInitstruct = {0};
  GPIO_InitTypeDef  GPIO_InitStruct = {0};

  /* Enable HSE (24MHz) */
  OscInitstruct.OscillatorType  = RCC_OSCILLATORTYPE_HSE;
  OscInitstruct.HSEState        = RCC_HSE_ON;                           /* Start HSE */
  OscInitstruct.HSEFreq         = RCC_HSE_16_32MHz;                     /* Choose HSE frequency of 16-32M */
                                                                        
  OscInitstruct.PLL.PLLState    = RCC_PLL_NONE;                         /* PLL does not handle it */
  if(HAL_RCC_OscConfig(&OscInitstruct) != HAL_OK)
  {
    APP_ErrorHandler();
  }

  /* The clock source selection for CAN is HSE (24MHz) */
  __HAL_RCC_CAN_CONFIG(RCC_CANCLKSOURCE_HSE); /* The clock source frequency of CAN is HSE (24MHz) */
  
  /* enable clock */
  __HAL_RCC_CAN1_CLK_ENABLE();  /* Enable CAN clock */
  __HAL_RCC_GPIOB_CLK_ENABLE(); /* Enable GPIOB clock */
  
  /* Configure CAN pins */
  GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9; /* CAN_RX:PB8,CAN_TX:PB9 */
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_CAN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
  /* NVIC interrupt enabling CANFD */
  HAL_NVIC_SetPriority(CAN_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(CAN_IRQn);
}

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
