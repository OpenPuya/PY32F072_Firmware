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
#include "py32f072xx_ll_Start_Kit.h"

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private user code ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static void APP_SystemClockConfig(void);
static void APP_SetSysclock(void);
static void APP_GPIOConfig(void);

/**
  * @brief  Main program.
  * @retval int
  */
int main(void)
{
  /* Initialization */
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

  /* Clock initialization, configure the system clock as LSI */
  APP_SystemClockConfig();
  
  /* Initialize button */
  BSP_PB_Init(BUTTON_USER,BUTTON_MODE_GPIO);
  
  /* Initialize GPIO for output */
  APP_GPIOConfig();
  
  /* MCO output clock and division initialization */
  LL_RCC_ConfigMCO(LL_RCC_MCO1SOURCE_SYSCLK,LL_RCC_MCO1_DIV_1);
  
  /*  Wait for the button to be pressed  */
  while(BSP_PB_GetState(BUTTON_USER) != 0)
  {
  }
  
  /* Change system clock to HSE */
  APP_SetSysclock();

  while (1)
  {
  }
}

/**
  * @brief  System clock configuration function
  * @param  None
  * @retval None
  */
static void APP_SystemClockConfig(void)
{
  /* Enable and initialize HSI */
  LL_RCC_HSI_Enable();
  LL_RCC_HSI_SetCalibFreq(LL_RCC_HSICALIBRATION_24MHz);
  while(LL_RCC_HSI_IsReady() != 1)
  {
  }
  
  /* Enable and initialize LSI */
  LL_RCC_LSI_Enable();
  while(LL_RCC_LSI_IsReady() != 1)
  {
  }
  
  /* Enable and initialize HSE */
  LL_RCC_HSE_Enable();
  LL_RCC_HSE_SetFreqRegion(LL_RCC_HSE_16_32MHz);
  while(LL_RCC_HSE_IsReady() != 1)
  {
  }
  
  /* Configure AHB prescaler */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);

  /* Configure LSI as system clock and initialize */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_LSI);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_LSI)
  {
  }

  /*Configure APB1 prescaler and initialize */
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_Init1msTick(LSI_VALUE);
  
  /* Update system clock global variable SystemCoreClock (can also be updated by calling SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(LSI_VALUE);
}

/**
  * @brief  Configure HSE as system clock
  * @param  None
  * @retval None
  */
static void APP_SetSysclock(void)
{
  /* Configure HSE as system clock and initialize */
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_HSE);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_HSE)
  {
  }
  
  /* Update system clock global variable SystemCoreClock (can also be updated by calling SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(HSE_VALUE);
  
  LL_Init1msTick(HSE_VALUE);
}

/**
  * @brief  Configure PA08 for MCO alternate function
  * @param  None
  * @retval None
  */
static void APP_GPIOConfig(void)
{
  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  /* Enable GPIOA clock */
  LL_IOP_GRP1_EnableClock(LL_IOP_GRP1_PERIPH_GPIOA);
  
  /* Configure PA08 for MCO alternate function */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_8;                  /* Select pin 8 */
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;        /* Configure as alternate function */
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH; /* Select output speed */
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL; /* Select output mode */
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;               /* No pull-up/down */
  GPIO_InitStruct.Alternate = LL_GPIO_AF0_MCO;          /* Select alternate function AF0 */
  LL_GPIO_Init(GPIOA,&GPIO_InitStruct);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void APP_ErrorHandler(void)
{
  /* Infinite loop */
  while(1)
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
     for example: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
