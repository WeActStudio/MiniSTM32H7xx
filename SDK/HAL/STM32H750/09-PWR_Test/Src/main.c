/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void LED_Blink(uint32_t Hdelay,uint32_t Ldelay)
{
	HAL_GPIO_WritePin(PE3_GPIO_Port,PE3_Pin,GPIO_PIN_SET);
	HAL_Delay(Hdelay - 1);
	HAL_GPIO_WritePin(PE3_GPIO_Port,PE3_Pin,GPIO_PIN_RESET);
	HAL_Delay(Ldelay-1);
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  /* USER CODE BEGIN 2 */
	
	if(__HAL_RCC_GET_FLAG(RCC_FLAG_SFTRST))
	{
		// System reset from CPU reset flag
		LED_Blink(500,200);
	}
	else if(__HAL_RCC_GET_FLAG(RCC_FLAG_PORRST))
	{
		// POR/PDR  reset
		LED_Blink(500,200);
		LED_Blink(500,200);
	}
	else if(__HAL_RCC_GET_FLAG(RCC_FLAG_PINRST))
	{
		// Pin reset
		LED_Blink(500,200);
		LED_Blink(500,200);
		LED_Blink(500,200);
	}
	__HAL_RCC_CLEAR_RESET_FLAGS();
	
	/* Check if the system was resumed from Standby mode */
  if (__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
  {
		/* Clear Standby flag */
	  __HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);
    /* Blink LED to indicate that the system was resumed from Standby mode */
    LED_Blink(10,250);
		LED_Blink(10,250);
  }
	
	if(HAL_PWREx_GetWakeupFlag(PWR_WAKEUP_FLAG4))
	{
		/* Blink LED to indicate that the system was resumed from Standby mode */
    LED_Blink(250,250);
		LED_Blink(250,250);
	}
	
	 /* The Following Wakeup sequence is highly recommended prior to each Standby mode entry
       mainly when using more than one wakeup source this is to not miss any wakeup event.
     - Disable all used wakeup sources,
     - Clear all related wakeup flags,
     - Re-enable all used wakeup sources,
     - Enter the Standby mode.
  */
	
  /* Disable used wakeup source: PWR_WAKEUP_PIN4 */
  HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN4);
	
	/* Clear related wakeup flags: PWR_WAKEUP_FLAG4 */
	HAL_PWREx_ClearWakeupFlag(PWR_WAKEUP_FLAG4);
	
  /* Enable WakeUp Pin PWR_WAKEUP_PIN4 connected to PC.13 */
  /* PWR_WAKEUP_PINx = Wake-Up x * 2 ,ex. Wake-Up 2 -> PWR_WAKEUP_PIN4 */
	PWREx_WakeupPinTypeDef sPinParams;
  sPinParams.WakeUpPin    = PWR_WAKEUP_PIN4;
  sPinParams.PinPolarity  = PWR_PIN_POLARITY_LOW;
  sPinParams.PinPull      = PWR_PIN_PULL_DOWN;
  HAL_PWREx_EnableWakeUpPin(&sPinParams);
  
	/* Clear related wakeup flags: PWR_WAKEUP_FLAG4 */
	HAL_PWREx_ClearWakeupFlag(PWR_WAKEUP_FLAG4);
	
  HAL_PWR_EnterSTANDBYMode();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		/* No Enter the StandBY Mode */
		LED_Blink(2,250);
		
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
