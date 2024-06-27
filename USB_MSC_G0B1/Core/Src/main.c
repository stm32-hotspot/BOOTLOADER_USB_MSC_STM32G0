/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include "fat_app.h"
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_storage_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef void (*pFunction)(void);
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern USBD_HandleTypeDef hUsbDeviceFS;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */
void JumpToApp(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */
	FLASH_EraseInitTypeDef flashErase;
	uint32_t ErrorFlash;
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

	/* Check whether the PC13 Button is released */
	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_SET)
	{
		/* ---------- Enter in Application ---------- */
		/* Jump to application */
		JumpToApp();
	}

	/* ---------- Enter in Bootloader mode ---------- */
	/* Fill the empty sector with 0s  */
	for(int i = 0; i < FAT_BLOCK_SIZE; i++)
	{
		blkSector[i] = 0;
	}

	/* Initialize the FAT File System */
	FAT_Init();

	/* Initialize the USB Device MSC */
	MX_USB_Device_Init();

	/* Turn the User LED ON to indicate that application entered in Bootloader */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		if(fatINFO.FileTransferCpt == SET)
		{
			/* Wait for the end of transmission */
			HAL_Delay(250);

			/* De-Initialize the USB Device */
			USBD_DeInit(&hUsbDeviceFS);

			/* Erase the Remaining Flash Memory */
			/* Initialize the Erase TypeDef */
			flashErase.Banks = FLASH_BANK_1;
			flashErase.TypeErase = FLASH_TYPEERASE_PAGES;

			/* Calculate the last Memory Position */
			flashErase.Page = (fatINFO.FileSize/FLASH_PAGE_SIZE) + 26;

			/* Unlock the Flash Memory */
			HAL_FLASH_Unlock();

			/* Check which bank corresponds to the calculated page */
			if(flashErase.Page < 127)
			{
				/* Compute the Number of pages */
				flashErase.NbPages = flashErase.Page - 24;

				if(HAL_FLASHEx_Erase(&flashErase, &ErrorFlash) != HAL_OK)
				{
					/* Lock the Flash Memory */
					HAL_FLASH_Lock();

					/* Call the Error Handler */
					Error_Handler();
				}

				/* Update the Erase settings */
				flashErase.Banks = FLASH_BANK_2;
				flashErase.Page = 256;
				flashErase.NbPages = 128;

				if(HAL_FLASHEx_Erase(&flashErase, &ErrorFlash) != HAL_OK)
				{
					/* Lock the Flash Memory */
					HAL_FLASH_Lock();

					/* Call the Error Handler */
					Error_Handler();
				}
			}
			else
			{
				/* Update the Erase settings */
				flashErase.Banks = FLASH_BANK_2;
				flashErase.Page += 129;
				flashErase.NbPages = 383 - flashErase.Page;

				if(HAL_FLASHEx_Erase(&flashErase, &ErrorFlash) != HAL_OK)
				{
					/* Lock the Flash Memory */
					HAL_FLASH_Lock();

					/* Call the Error Handler */
					Error_Handler();
				}
			}

			/* Lock the Flash Memory */
			HAL_FLASH_Lock();

			/* Jump To Application */
			JumpToApp();
		}
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

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSI48;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
	RCC_OscInitStruct.PLL.PLLN = 8;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

	/*Configure GPIO pin : PC13 */
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : PA5 */
	GPIO_InitStruct.Pin = GPIO_PIN_5;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
/**
 * @brief Perform a jump to the application if it's available
 * @param None
 * @retval None
 */
void JumpToApp(void)
{
	/* Local variables */
	uint32_t JumpAddress;
	pFunction Jump_To_Application;

	/* Check whether App is available */
	if((*(uint32_t*) FLASH_APP_ADDR) != 0xFFFFFFFF)
	{
		/* De-init the GPIOS */
		HAL_GPIO_DeInit(GPIOC, GPIO_PIN_13);
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);

		/* Set the Clock to default state */
		HAL_RCC_DeInit();

		/* Jump to user application */
		JumpAddress = *(__IO uint32_t*) (FLASH_APP_ADDR + 4);
		Jump_To_Application = (pFunction)JumpAddress;

		/* Initialize use application's Stack Pointer */
		__set_MSP(*(__IO uint32_t*)FLASH_APP_ADDR);
		Jump_To_Application();
	}
	else
	{
		/* Blink the LED at 0.5Hz to indicate that there isn't an application into the memory */
		while(1)
		{
			HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
			HAL_Delay(1000);
		}
	}
}

/**
 * @brief Perform a Flash Page Erase
 * @param page: the page number to be erased
 * @retval status: HAL_StatusTypeDef with the operation result
 */
HAL_StatusTypeDef EraseFlashPage(uint16_t page)
{
	/* Local Variables */
	HAL_StatusTypeDef status;
	FLASH_EraseInitTypeDef flashErase_t;
	uint32_t EraseError;

	/* Erase TypeDef initialization */
	flashErase_t.Page = page;
	flashErase_t.NbPages = 1;
	flashErase_t.TypeErase = FLASH_TYPEERASE_PAGES;

	if(page <= 127)
	{
		flashErase_t.Banks = FLASH_BANK_1;
	}
	else
	{
		flashErase_t.Banks = FLASH_BANK_2;
	}

	/* Unlock the Flash Memory */
	HAL_FLASH_Unlock();

	/* Perform the Flash Erase Page */
	status = HAL_FLASHEx_Erase(&flashErase_t, &EraseError);

	/* Lock the Flash Memory */
	HAL_FLASH_Lock();

	/* Return the operation result */
	return status;
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

	while (1)
	{
		/* Toggle the User LED status */
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);

		/* Wait for 500ms */
		HAL_Delay(500);

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
