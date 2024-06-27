/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : usbd_storage_if.c
 * @version        : v3.0_Cube
 * @brief          : Memory management layer.
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
#include "usbd_storage_if.h"

/* USER CODE BEGIN INCLUDE */
#include "fat_app.h"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
 * @brief Usb device.
 * @{
 */

/** @defgroup USBD_STORAGE
 * @brief Usb mass storage device module
 * @{
 */

/** @defgroup USBD_STORAGE_Private_TypesDefinitions
 * @brief Private types.
 * @{
 */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
 * @}
 */

/** @defgroup USBD_STORAGE_Private_Defines
 * @brief Private defines.
 * @{
 */

#define STORAGE_LUN_NBR                  1								// Number of Logical Unit Number
#define STORAGE_BLK_NBR                  FAT_BLOCK_NBR					// Storage Block Number
#define STORAGE_BLK_SIZ                  FAT_BLOCK_SIZE					// Storage Block Size

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
 * @}
 */

/** @defgroup USBD_STORAGE_Private_Macros
 * @brief Private macros.
 * @{
 */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
 * @}
 */

/** @defgroup USBD_STORAGE_Private_Variables
 * @brief Private variables.
 * @{
 */

/* USER CODE BEGIN INQUIRY_DATA_FS */
/** USB Mass storage Standard Inquiry Data. */
const int8_t STORAGE_Inquirydata_FS[] = {/* 36 */

		/* LUN 0 */
		0x00,
		0x80,
		0x02,
		0x02,
		(STANDARD_INQUIRY_DATA_LEN - 5),
		0x00,
		0x00,
		0x00,
		'S', 'T', 'M', ' ', ' ', ' ', ' ', ' ', /* Manufacturer : 8 bytes */
		'P', 'r', 'o', 'd', 'u', 'c', 't', ' ', /* Product      : 16 Bytes */
		' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ',
		'0', '.', '0' ,'1'                      /* Version      : 4 Bytes */
};
/* USER CODE END INQUIRY_DATA_FS */

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
 * @}
 */

/** @defgroup USBD_STORAGE_Exported_Variables
 * @brief Public variables.
 * @{
 */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
 * @}
 */

/** @defgroup USBD_STORAGE_Private_FunctionPrototypes
 * @brief Private functions declaration.
 * @{
 */

static int8_t STORAGE_Init_FS(uint8_t lun);
static int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_FS(uint8_t lun);
static int8_t STORAGE_IsWriteProtected_FS(uint8_t lun);
static int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS(void);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */
ITStatus isValidFile(FAT_BaseEntry_t *file);
/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
 * @}
 */

USBD_StorageTypeDef USBD_Storage_Interface_fops_FS =
{
		STORAGE_Init_FS,
		STORAGE_GetCapacity_FS,
		STORAGE_IsReady_FS,
		STORAGE_IsWriteProtected_FS,
		STORAGE_Read_FS,
		STORAGE_Write_FS,
		STORAGE_GetMaxLun_FS,
		(int8_t *)STORAGE_Inquirydata_FS
};

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Initializes the storage unit (medium) over USB FS IP
 * @param  lun: Logical unit number.
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t STORAGE_Init_FS(uint8_t lun)
{
	/* USER CODE BEGIN 2 */
	UNUSED(lun);

	return (USBD_OK);
	/* USER CODE END 2 */
}

/**
 * @brief  Returns the medium capacity.
 * @param  lun: Logical unit number.
 * @param  block_num: Number of total block number.
 * @param  block_size: Block size.
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
	/* USER CODE BEGIN 3 */
	UNUSED(lun);

	*block_num  = STORAGE_BLK_NBR;
	*block_size = STORAGE_BLK_SIZ;
	return (USBD_OK);
	/* USER CODE END 3 */
}

/**
 * @brief   Checks whether the medium is ready.
 * @param  lun:  Logical unit number.
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t STORAGE_IsReady_FS(uint8_t lun)
{
	/* USER CODE BEGIN 4 */
	UNUSED(lun);

	return (USBD_OK);
	/* USER CODE END 4 */
}

/**
 * @brief  Checks whether the medium is write protected.
 * @param  lun: Logical unit number.
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t STORAGE_IsWriteProtected_FS(uint8_t lun)
{
	/* USER CODE BEGIN 5 */
	UNUSED(lun);

	return (USBD_OK);
	/* USER CODE END 5 */
}

/**
 * @brief  Reads data from the medium.
 * @param  lun: Logical unit number.
 * @param  buf: data buffer.
 * @param  blk_addr: Logical block address.
 * @param  blk_len: Blocks number.
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
	/* USER CODE BEGIN 6 */
	/* Reading data from RAM memory */
	if(blk_addr <= FAT_RAM_ALL_NBR)
	{
		memcpy(buf, &ramBuffer[blk_addr*STORAGE_BLK_SIZ],blk_len*STORAGE_BLK_SIZ);
	}
	/* Reading from FLASH memory */
	else
	{
		/* Copy the blank sector to the buffer */
		for(int i = 0; i < blk_len; i++)
		{
			memcpy(buf, blkSector, STORAGE_BLK_SIZ);
			buf++;
		}
	}

	return(USBD_OK);
	/* USER CODE END 6 */
}

/**
 * @brief  Writes data into the medium.
 * @param  lun: Logical unit number.
 * @param  buf: data buffer.
 * @param  blk_addr: Logical block address.
 * @param  blk_len: Blocks number.
 * @retval USBD_OK if all operations are OK else USBD_FAIL
 */
int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
	/* USER CODE BEGIN 7 */
	/* Writing in RAM memory */
	if(blk_addr <= FAT_RAM_ALL_NBR)
	{
		/* Copying received content into ramBuffer */
		memcpy(&ramBuffer[blk_addr*STORAGE_BLK_SIZ], buf, blk_len*STORAGE_BLK_SIZ);
	}

	/* Check if the block address is already on the Root Directory Section */
	if(blk_addr == fatINFO.FirstRootDirSecNum)
	{
		/* Auxiliar pointer to manage the ramBuffer address */
		uint32_t *buff = (uint32_t*)&ramBuffer[blk_addr*STORAGE_BLK_SIZ];

		/* Runs through the page looking for the "UPDF" value (0x46445055) to start writing */
		for(int entryCount = 0; entryCount < FAT_BLOCK_SIZE; entryCount++)
		{
			if(*buff == FAT_NAME_FILE && fatINFO.ReceivedFile != SET)
			{
				/* Compute the Base Entry for the Binary file */
				binFile = (FAT_BaseEntry_t *)&ramBuffer[(FAT_BLOCK_SIZE * fatINFO.FirstRootDirSecNum) +
														(FAT_BASEENTRY_SIZE * entryCount)];

				if(isValidFile(binFile) == SET)
				{
					/* Compute the Sector which Binary file starts */
					fatINFO.FirstDataPosition = fatINFO.FirstDataSector + (binFile->FirstClusterHI[1] << 8) +
							binFile->FirstClusterLO[0] - FAT_START_CLUSTER;

					/* Compute the File Size */
					fatINFO.FileSize = (binFile->Size[3] << 24) + (binFile->Size[2] << 16) + (binFile->Size[1] << 8) +
							binFile->Size[0];

					/* Update the received file flag */
					fatINFO.ReceivedFile = SET;

					/* Break the for loop */
					break;
				}
			}

			/* Increase the buffer pointer */
			buff += 8;
		}
	}

	/* Writing in FLASH memory */
	if((blk_addr >= fatINFO.FirstDataPosition) && (fatINFO.ReceivedFile == SET))
	{
		/* Local variables*/
		uint16_t flashPage 		= blk_addr - fatINFO.FirstDataPosition + 25;
		uint32_t flashAddress 	= (flashPage * FLASH_PAGE_SIZE) + FLASH_BASE;
		uint64_t *fileData 		= (uint64_t *)buf;

		/* To update the parameters when it comes to Bank_2 */
		if(flashPage > 127)
		{
			flashPage += 129;
		}

		/* Writing Content in FLASH */
		if(fatINFO.FileTransferCpt != SET)
		{
			/* Erase the Flash to be Programmed */
			if(EraseFlashPage(flashPage) != HAL_OK)
			{
				/* Call the Error Handler Function in case of error */
				Error_Handler();
			}

			/* Unlock the Flash to be Programmed */
			HAL_FLASH_Unlock();

			/* Perform the Flash program */
			for(int i = 0; i < 256; i++)
			{
				if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, flashAddress, *fileData) != HAL_OK)
				{
					/* Lock the Flash */
					HAL_FLASH_Lock();

					/* Call the Error Handler Function in case of error */
					Error_Handler();
				}

				/* Wait the flash to be ready */
				FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

				/* Increase the Data Pointer and Flash Memory Address */
				fileData++;
				flashAddress += 8;
			}

			/* Lock the Flash */
			HAL_FLASH_Lock();

			/* Compute the Transferred Data */
			fatINFO.TransferredData += FAT_BLOCK_SIZE;
		}

		/* Check whether all data was programmed */
		if(fatINFO.TransferredData >= fatINFO.FileSize)
		{
			/* Set the Transfer Complete Flag */
			fatINFO.FileTransferCpt = SET;
		}

	}

	return(USBD_OK);
	/* USER CODE END 7 */
}

/**
 * @brief  Returns the Max Supported LUNs.
 * @param  None
 * @retval Lun(s) number.
 */
int8_t STORAGE_GetMaxLun_FS(void)
{
	/* USER CODE BEGIN 8 */
	return (STORAGE_LUN_NBR - 1);
	/* USER CODE END 8 */
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */
/**
 * @brief  Checks whether the respective BaseEntry points to a valid bin file
 * @param  file: pointer to a Base Entry
 * @retval validFile: Flag to indicate if it's a valid file
 */
ITStatus isValidFile(FAT_BaseEntry_t *file)
{
	/* Local Variables */
	ITStatus validFile 				= SET;
	uint32_t *fileName 				= (uint32_t *)file->Name,
			 *fileExtension 		= (uint32_t *)&file->Name[8],
			 fileSize				= 0;

	/* Compute the File Size */
	fileSize = (binFile->Size[3] << 24) + (binFile->Size[2] << 16) + (binFile->Size[1] << 8) +
			binFile->Size[0];

	/* Check the name, the extension and the attributes of the file */
	if((*fileName != FAT_NAME_FILE) || ((*fileExtension & 0x00FFFFFF) != FAT_BIN_EXTENSION) || (file->Attribute != 0x20) || (fileSize == 0))
	{
		validFile = RESET;
	}

	/* Return if it's a valid file */
	return validFile;
}
/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
 * @}
 */

/**
 * @}
 */

