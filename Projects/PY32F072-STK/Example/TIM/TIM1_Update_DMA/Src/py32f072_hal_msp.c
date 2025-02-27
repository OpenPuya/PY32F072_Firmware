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
static DMA_HandleTypeDef  hdma_tim;

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
  __HAL_RCC_DMA_CLK_ENABLE();                                 /* Enable DMA clock */
  __HAL_RCC_TIM1_CLK_ENABLE();                                /* Enable TIM1 clock */
  __HAL_RCC_SYSCFG_CLK_ENABLE();                              /* Enable SYSCFG clock */

  hdma_tim.Instance = DMA1_Channel1;                          /* Select DMA channel 1 */
  hdma_tim.Init.Direction = DMA_MEMORY_TO_PERIPH;             /* Memory to peripheral direction */
  hdma_tim.Init.PeriphInc = DMA_PINC_DISABLE;                 /* Peripheral increment mode Disable */
  hdma_tim.Init.MemInc = DMA_MINC_ENABLE;                     /* Memory increment mode Enable */
  hdma_tim.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD ;   /* Peripheral data alignment : Word */
  hdma_tim.Init.MemDataAlignment = DMA_MDATAALIGN_WORD ;      /* Memory data alignment : Word */
  hdma_tim.Init.Mode = DMA_NORMAL;                            /* Circular mode disble */
  hdma_tim.Init.Priority = DMA_PRIORITY_VERY_HIGH;            /* Priority level : Very_High */
  HAL_NVIC_SetPriority(TIM1_BRK_UP_TRG_COM_IRQn, 0, 0);       /* Set the interrupt priority */
  HAL_NVIC_EnableIRQ(TIM1_BRK_UP_TRG_COM_IRQn);               /* Enable TIM1 interrupt */
  HAL_DMA_ChannelMap(&hdma_tim,DMA_CHANNEL_MAP_TIM1_UP);      /* DMA1_MAP select TIM1_UP */

  __HAL_LINKDMA(htim, hdma[TIM_DMA_ID_UPDATE], hdma_tim);     /* DMA1 is associated TIM_UP event */
  HAL_DMA_Init(htim->hdma[TIM_DMA_ID_UPDATE]);                /* Initialize DMA */
  
  /*Start DMA*/
  HAL_DMA_Start(htim->hdma[TIM_DMA_ID_UPDATE], (uint32_t)&Arr_DMA, (uint32_t)&TIM1->ARR, 3);
}

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
