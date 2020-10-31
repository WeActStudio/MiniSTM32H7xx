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
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_2_BANK1      /* Start @ of user Flash area Bank1 */
#define FLASH_USER_END_ADDR     (ADDR_FLASH_SECTOR_7_BANK1 - 1)  /* End @ of user Flash area Bank1*/

uint32_t FirstSector = 0, NbOfSectors = 0;
uint32_t Address = 0, SECTORError = 0, Index = 0;
__IO uint32_t MemoryProgramStatus = 0;
__IO uint64_t data64 = 0;

uint64_t FlashWord[4] = { 0x0102030405060708,
                          0x1112131415161718,
                          0x2122232425262728,    
                          0x3132333435363738
                        };
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static FLASH_EraseInitTypeDef EraseInitStruct;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static uint32_t GetSector(uint32_t Address);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/**
* @brief  CPU L1-Cache enable.
* @param  None
* @retval None
*/
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

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
  CPU_CACHE_Enable();
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
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		  /* -2- Unlock the Flash to enable the flash control register access *************/
		HAL_FLASH_Unlock();

		/* -3- Erase the user Flash area
			(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

		/* Get the 1st sector to erase */
		FirstSector = GetSector(FLASH_USER_START_ADDR);
		/* Get the number of sector to erase from 1st sector*/
		NbOfSectors = GetSector(FLASH_USER_END_ADDR) - FirstSector + 1;

		/* Fill EraseInit structure*/
		EraseInitStruct.TypeErase     = FLASH_TYPEERASE_SECTORS;
		EraseInitStruct.VoltageRange  = FLASH_VOLTAGE_RANGE_3;
		EraseInitStruct.Banks         = FLASH_BANK_1;
		EraseInitStruct.Sector        = FirstSector;
		EraseInitStruct.NbSectors     = NbOfSectors;
		
		if (HAL_FLASHEx_Erase(&EraseInitStruct, &SECTORError) != HAL_OK)
		{
			/*
				Error occurred while sector erase.
				User can add here some code to deal with this error.
				SECTORError will contain the faulty sector and then to know the code error on this sector,
				user can call function 'HAL_FLASH_GetError()'
			*/
			/* Infinite loop */
			while (1)
			{
				LED_Blink(500,500);
			}
		}

		/* -4- Program the user Flash area word by word
			(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

		Address = FLASH_USER_START_ADDR;

		while (Address < FLASH_USER_END_ADDR)
		{
			if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD, Address, ((uint32_t)FlashWord)) == HAL_OK)
			{
				Address = Address + 32; /* increment for the next Flash word*/
			}
			else
			{
				/* Error occurred while writing data in Flash memory.
					 User can add here some code to deal with this error */
				while (1)
				{
					LED_Blink(500,500);
				}
			}
		}

		/* -5- Lock the Flash to disable the flash control register access (recommended
			 to protect the FLASH memory against possible unwanted operation) *********/
		HAL_FLASH_Lock();

		/* -6- Check if the programmed data is OK
				MemoryProgramStatus = 0: data programmed correctly
				MemoryProgramStatus != 0: number of words not programmed correctly ******/
		Address = FLASH_USER_START_ADDR;
		MemoryProgramStatus = 0x0;

		while (Address < FLASH_USER_END_ADDR)
		{
			for(Index = 0; Index<4; Index++)
			{
				data64 = *(uint64_t*)Address;
				__DSB();
				if(data64 != FlashWord[Index])
				{
					MemoryProgramStatus++;
				}
				Address +=8;
			}
		}

		/* -7- Check if there is an issue to program data*/
		if (MemoryProgramStatus == 0)
		{
			/* No error detected. Switch on LED1*/
			LED_Blink(3000,500);
		}
		else
		{
			/* Error detected. Toggle LED3*/
			while (1)
				{
					LED_Blink(500,500);
				}
		}

		/* Infinite loop */
		while (1)
		{
			LED_Blink(50,500);
		}
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
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Gets the sector of a given address
  * @param  Address Address of the FLASH Memory
  * @retval The sector of a given address
  */
uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if(((Address < ADDR_FLASH_SECTOR_1_BANK1) && (Address >= ADDR_FLASH_SECTOR_0_BANK1)) || \
     ((Address < ADDR_FLASH_SECTOR_1_BANK2) && (Address >= ADDR_FLASH_SECTOR_0_BANK2)))    
  {
    sector = FLASH_SECTOR_0;  
  }
  else if(((Address < ADDR_FLASH_SECTOR_2_BANK1) && (Address >= ADDR_FLASH_SECTOR_1_BANK1)) || \
          ((Address < ADDR_FLASH_SECTOR_2_BANK2) && (Address >= ADDR_FLASH_SECTOR_1_BANK2)))    
  {
    sector = FLASH_SECTOR_1;  
  }
  else if(((Address < ADDR_FLASH_SECTOR_3_BANK1) && (Address >= ADDR_FLASH_SECTOR_2_BANK1)) || \
          ((Address < ADDR_FLASH_SECTOR_3_BANK2) && (Address >= ADDR_FLASH_SECTOR_2_BANK2)))    
  {
    sector = FLASH_SECTOR_2;  
  }
  else if(((Address < ADDR_FLASH_SECTOR_4_BANK1) && (Address >= ADDR_FLASH_SECTOR_3_BANK1)) || \
          ((Address < ADDR_FLASH_SECTOR_4_BANK2) && (Address >= ADDR_FLASH_SECTOR_3_BANK2)))    
  {
    sector = FLASH_SECTOR_3;  
  }
  else if(((Address < ADDR_FLASH_SECTOR_5_BANK1) && (Address >= ADDR_FLASH_SECTOR_4_BANK1)) || \
          ((Address < ADDR_FLASH_SECTOR_5_BANK2) && (Address >= ADDR_FLASH_SECTOR_4_BANK2)))    
  {
    sector = FLASH_SECTOR_4;  
  }
  else if(((Address < ADDR_FLASH_SECTOR_6_BANK1) && (Address >= ADDR_FLASH_SECTOR_5_BANK1)) || \
          ((Address < ADDR_FLASH_SECTOR_6_BANK2) && (Address >= ADDR_FLASH_SECTOR_5_BANK2)))    
  {
    sector = FLASH_SECTOR_5;  
  }
  else if(((Address < ADDR_FLASH_SECTOR_7_BANK1) && (Address >= ADDR_FLASH_SECTOR_6_BANK1)) || \
          ((Address < ADDR_FLASH_SECTOR_7_BANK2) && (Address >= ADDR_FLASH_SECTOR_6_BANK2)))    
  {
    sector = FLASH_SECTOR_6;  
  }
  else if(((Address < ADDR_FLASH_SECTOR_0_BANK2) && (Address >= ADDR_FLASH_SECTOR_7_BANK1)) || \
          ((Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_7_BANK2)))
  {
     sector = FLASH_SECTOR_7;  
  }
  else
  {
    sector = FLASH_SECTOR_7;
  }

  return sector;
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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
