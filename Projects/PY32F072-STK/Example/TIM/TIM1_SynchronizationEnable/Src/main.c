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
TIM_HandleTypeDef    TimHandle1, TimHandle3;
TIM_SlaveConfigTypeDef   sSlaveConfig;
TIM_MasterConfigTypeDef sMasterConfig;
TIM_OC_InitTypeDef sConfig;

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
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
  /* Reset of all peripherals, Initializes the Systick */
  HAL_Init();
  
  /* Configure Systemclock */
  APP_SystemClockConfig(); 
  
  /* Initialize LED */
  BSP_LED_Init(LED_GREEN);

  TimHandle1.Instance = TIM1;                                           /* Select TIM1 */
  TimHandle1.Init.Period            = 8000 - 1;                         /* Auto reload value： */
  TimHandle1.Init.Prescaler         = 100 - 1;                          /* Prescaler:100-1 */
  TimHandle1.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;           /* Clock division: tDTS=tCK_INT */
  TimHandle1.Init.CounterMode       = TIM_COUNTERMODE_UP;               /* CounterMode:Up */
  TimHandle1.Init.RepetitionCounter = 1 - 1;                            /* repetition counter value:1-1 */
  TimHandle1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;   /* TIM1_ARR register is not buffered */
  /* Initialize TIM1 */
  if (HAL_TIM_Base_Init(&TimHandle1) != HAL_OK)
  {
    APP_ErrorHandler();
  }

  TimHandle3.Instance = TIM3;                                               /* Select TIM3 */
  TimHandle3.Init.Prescaler = 1000 - 1;                                     /* Prescaler:1000-1 */
  TimHandle3.Init.CounterMode = TIM_COUNTERMODE_UP;                         /* CounterMode:Up */
  TimHandle3.Init.Period = 8000 - 1;                                        /* Auto reload value： */
  TimHandle3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;                   /* Clock division: tDTS=tCK_INT */
  TimHandle3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;       /* TIM1_ARR register is not buffered */
  /* Initializes the TIM Output Compare */
  if (HAL_TIM_OC_Init(&TimHandle3) != HAL_OK)
  {
    APP_ErrorHandler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;                 /* Update event is used as trigger output (TRGO) */
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;         /* The master/slave mode is disabled */
  /* Configures the TIM3 in master mode. */
  HAL_TIMEx_MasterConfigSynchronization(&TimHandle3, &sMasterConfig);

  sSlaveConfig.SlaveMode        = TIM_SLAVEMODE_TRIGGER;               /* Slave mode is trigger mode */
  sSlaveConfig.InputTrigger     = TIM_TS_ITR2;                         /* The trigger selection for TIM1 is TIM3 */
  sSlaveConfig.TriggerPolarity  = TIM_TRIGGERPOLARITY_NONINVERTED;     /* ETR is non-inverted, active at high level or rising edge */
  sSlaveConfig.TriggerPrescaler = TIM_TRIGGERPRESCALER_DIV1;           /* Prescaler OFF */
  sSlaveConfig.TriggerFilter    = 0;                                   /* No filter */
  /* Configures the TIM1 in Slave mode */
  if (HAL_TIM_SlaveConfigSynchro(&TimHandle1, &sSlaveConfig) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  /* Enable TIM1 update interrupt */
  __HAL_TIM_ENABLE_IT(&TimHandle1, TIM_IT_UPDATE);
  
  /* Starts the TIM3 Output Compare signal generation. */
  if (HAL_TIM_OC_Start(&TimHandle3, TIM_CHANNEL_1) != HAL_OK)
  {
    APP_ErrorHandler();
  }
  
  /* Infinite loop*/
  while (1)
  {
  }

}

/**
  * @brief  TIMUpdate interrupt callback
  * @param  htim：TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* Toggle LED */
  BSP_LED_Toggle(LED_GREEN);
}

/**
  * @brief  Configure system clock
  * @param  None
  * @retval None
  */
void APP_SystemClockConfig(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /* RCC Oscillators configuration */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE | RCC_OSCILLATORTYPE_HSI | RCC_OSCILLATORTYPE_LSI | RCC_OSCILLATORTYPE_LSE; /* Select oscillators HSE,HSI,LSI,LSE */
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;                          /* HSI ON */
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;                          /* HSI clock is not divided */
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_8MHz;  /* Configure HSI to 8Mhz */
  RCC_OscInitStruct.HSEState = RCC_HSE_OFF;                         /* HSE OFF */
  /*RCC_OscInitStruct.HSEFreq = RCC_HSE_16_32MHz;*/
  RCC_OscInitStruct.LSIState = RCC_LSI_OFF;                         /* LSI OFF */
  RCC_OscInitStruct.LSEState = RCC_LSE_OFF;                         /* LSE OFF */
  /*RCC_OscInitStruct.LSEDriver = RCC_LSEDRIVE_MEDIUM;*/
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_OFF;                     /* PLL OFF */
  /*RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_NONE;*/
  /*RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;*/
  /* Initialize RCC Oscillators */
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    APP_ErrorHandler();
  }

  /* Clock source configuration */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1; /* Choose to configure HCLK,SYSCLK,PCLK1 */
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSISYS; /* Select HSISYS as system clock */
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;     /* SYSCLK not divided */
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;      /* HCLK not divided */
  /* Configure Clock source */
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    APP_ErrorHandler();
  }
}

/**
  * @brief  Error handling function
  * @param  None
  * @retval None
  */
void APP_ErrorHandler(void)
{
  /* Infinite loop */
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file：Pointer to the source file name
  * @param  line：assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* User can add His own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* Infinite loop */
  while (1)
  {
  }
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT Puya *****END OF FILE******************/
