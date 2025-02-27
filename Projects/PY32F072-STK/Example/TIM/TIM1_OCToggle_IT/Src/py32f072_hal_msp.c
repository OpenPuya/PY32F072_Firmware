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
  * @brief Initialize the MSP
  */
void HAL_MspInit(void)
{
  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();
}

/**
  * @brief Initialize TIM1 related MSP
  */
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
  GPIO_InitTypeDef   GPIO_InitStruct = {0};

  /* Enable TIM1 clock */
  __HAL_RCC_TIM1_CLK_ENABLE();                              
  /* Enable GPIOA clock */
  __HAL_RCC_GPIOA_CLK_ENABLE();                             
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;         /* Alternate Function Push Pull Mode */
  GPIO_InitStruct.Pull = GPIO_PULLUP;             /* Pull up */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  /* Initialize GPIOA8 */
  GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL1;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
  HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL1, &GPIO_InitStruct);
  /* Initialize GPIOA9 */
  GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL2;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
  HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL2, &GPIO_InitStruct);
  /* Initialize GPIOA10 */
  GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL3;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
  HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL3, &GPIO_InitStruct);
  /* Initialize GPIOA11 */
  GPIO_InitStruct.Pin = TIMx_GPIO_PIN_CHANNEL4;
  GPIO_InitStruct.Alternate = GPIO_AF2_TIM1;
  HAL_GPIO_Init(TIMx_GPIO_PORT_CHANNEL4, &GPIO_InitStruct);
  
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;         /* Push pull mode */
  GPIO_InitStruct.Pull = GPIO_NOPULL;                 /* No Pull-up or Pull-down activation */
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;

  GPIO_InitStruct.Pin = GPIO_PIN_4;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    
  /* Set the capture/compare interrupt priority */
  HAL_NVIC_SetPriority(TIM1_CC_IRQn, 1,0);
  /* Enable capture/compare interrupt in NVIC */
  HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);
  
  /* Set the update interrupt priority */
  HAL_NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 1,0);
  /* Enable update interrupt in NVIC */
  HAL_NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);
}

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
