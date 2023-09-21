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
#include "rtc.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "quadspi.h"
#include "w25qxx_qspi.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

#define W25Qxx_TEST              (0)
#define APPLICATION_ADDRESS      QSPI_BASE
#define ISP_ADDRESS              (0x1FF09800UL)
typedef  void (*pFunction)(void);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
pFunction JumpToApplication;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void CPU_CACHE_Disable(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#if W25Qxx_TEST == (1)
uint8_t read[4096];
uint8_t write[4096];
#endif

uint8_t app_IsReady(uint32_t addr)
{
  uint32_t data;
  
  W25qxx_Read((uint8_t *)&data,addr - QSPI_BASE,sizeof(data));
  
	if((data & 0x2FF80000 ) == 0x24000000)
		return SUCCESS;
	else if((data & 0x2FF80000 ) == 0x20000000)
		return SUCCESS;
  else if((data & 0x3FF80000 ) == 0x30000000)
		return SUCCESS;
 	else if((data & 0x3FF80000 ) == 0x00000000)
		return SUCCESS; 
	else
		return ERROR;
}

void app_Jump(uint32_t addr)
{
	pFunction JumpToApplication;
	uint32_t JumpAddress;
	
	/* Jump to user application */
	JumpAddress = *(__IO uint32_t*) (addr + 4);
	JumpToApplication = (pFunction) JumpAddress;

	/* Initialize user application's Stack Pointer */
	__set_MSP(*(__IO uint32_t*) addr);
	JumpToApplication();			
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

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

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
  MX_RTC_Init();
  /* USER CODE BEGIN 2 */
	
	/* QPIMode && NormalMode : 100Mhz Clk can run to App, Normal */
	/* QPIMode && DTRMode    : 100Mhz Clk can not run to App, need lower Clk, Recommend 50Mhz */
	/* SPIMode && NormalMode : 100Mhz Clk can run to App, Normal */
	/* SPIMode && DTRMode    : 100Mhz Clk can not run to App, need lower Clk, Recommend 50Mhz*/
	
	/* QPIMode && NormalMode : 120Mhz Clk can not run to App, UnNormal */
	/* QPIMode && DTRMode    : 120Mhz Clk can not run to App, need lower Clk, Recommend 60Mhz */
	/* SPIMode && NormalMode : 120Mhz Clk can run to App, Normal */
	/* SPIMode && DTRMode    : 120Mhz Clk can not run to App, need lower Clk, Recommend 60Mhz*/
	
	w25qxx_Init();
	w25qxx_EnterQPI();
	
#if W25Qxx_TEST == (1)
	W25qxx_EraseSector(0);
	for(uint32_t j=0;j<sizeof(write);j+=256)
		for(uint32_t i=0;i<=0xff;i++)
			write[j+i] = i;
	W25qxx_WriteNoCheck(write,0,sizeof(write));
	W25qxx_Read(read,0,sizeof(read));
#endif
	
#if W25Qxx_TEST == (0)
  /* Reset MCU && Long Press K1 to Enter ISP Mode */
	if(HAL_GPIO_ReadPin(K1_GPIO_Port,K1_Pin) == GPIO_PIN_SET)
	{
		HAL_Delay(50);
		while(HAL_GPIO_ReadPin(K1_GPIO_Port,K1_Pin) == GPIO_PIN_SET)
		{
			HAL_GPIO_TogglePin(PE3_GPIO_Port,PE3_Pin);
			HAL_Delay(80);
		}
		HAL_QSPI_DeInit(&hqspi);
		HAL_GPIO_DeInit(GPIOA,GPIO_PIN_All);
		HAL_GPIO_DeInit(GPIOB,GPIO_PIN_All);
		HAL_GPIO_DeInit(GPIOC,GPIO_PIN_All);
		HAL_GPIO_DeInit(GPIOD,GPIO_PIN_All);
		HAL_GPIO_DeInit(GPIOE,GPIO_PIN_All);
		
		/* Disable CPU L1 cache before jumping to the QSPI code execution */
		CPU_CACHE_Disable();
		/* Disable Systick interrupt */
		SysTick->CTRL = 0;
		
		/* Initialize ISP application's Stack Pointer & Jump to ISP application */
		app_Jump(ISP_ADDRESS);
	}
	
  if(app_IsReady(APPLICATION_ADDRESS) == SUCCESS)
	{
		/* Enter memory_mapped mode*/
		w25qxx_Startup(w25qxx_DTRMode);
		/* Disable CPU L1 cache before jumping to the QSPI code execution */
		CPU_CACHE_Disable();
		/* Disable Systick interrupt */
		SysTick->CTRL = 0;

		/* Initialize user application's Stack Pointer & Jump to user application */
		app_Jump(APPLICATION_ADDRESS);
	}
#endif	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		HAL_GPIO_TogglePin(PE3_GPIO_Port,PE3_Pin);
		HAL_Delay(500);
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

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 5;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = 2;
  RCC_OscInitStruct.PLL.PLLQ = 1;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
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
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV1;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
  * @brief  CPU L1-Cache disable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Disable(void)
{
  /* Disable I-Cache */
  SCB_DisableICache();

  /* Disable D-Cache */
  SCB_DisableDCache();
}

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
