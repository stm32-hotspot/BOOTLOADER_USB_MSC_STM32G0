/*
 * fat_header_descriptor.h
 *
 *  Created on: Dec 22, 2022
 *      Author: botelhod
 */

#ifndef INC_FAT_APP_H_
#define INC_FAT_APP_H_

/* Includes */
#include <stdint.h>
#include "main.h"

/* Exported Defines */
#define FAT_BLOCK_NBR		0xF6			// Number of Blocks (Sectors)
#define FAT_BLOCK_SIZE		0x800			// Size of Blocks (Sectors)
#define FAT_RAM_ALL_NBR		0x30			// Number of Blocks allocated on RAM (should be at least 5)
#define FAT_START_CLUSTER 	0x02			// Cluster offset (the cluster count starts on 2)
#define FAT_BASEENTRY_SIZE	32				// Length of BASE ENTRY in bytes
#define FAT_NAME_FILE		0x46445055		//FILE NAME = "UPDFxxxx" - It can be changed and should be in little endian
#define FAT_BIN_EXTENSION	0x004E4942		//FILE EXTENSION = "BIN"

/* Exported Types */
typedef struct		/* FAT Boot Sector and BIOS Parameter Block Struct Typedef */
{
	uint8_t jmpBoot[3];
	uint8_t OEMName[8];
	uint8_t BytsPerSec[2];
	uint8_t SecPerClus;
	uint8_t ResvdSecCnt[2];
	uint8_t NumFATs;
	uint8_t RootEntCnt[2];
	uint8_t TotSec16[2];
	uint8_t Media;
	uint8_t FATSz16[2];
	uint8_t SecPerTrk[2];
	uint8_t NumHeads[2];
	uint8_t HiddSec[4];
	uint8_t TotSec32[4];
	uint8_t DrvNum;
	uint8_t Reserved1;
	uint8_t BootSig;
	uint8_t VolID[4];
	uint8_t VolLab[11];
	uint8_t FilSysType[8];
}FAT_BootSector_t;

typedef struct /* FAT System Info Struct Typedef */
{
	FlagStatus ReceivedFile;
	FlagStatus FileTransferCpt;
	uint16_t RootDirSectors;
	uint16_t FirstDataSector;
	uint16_t FirstRootDirSecNum;
	uint16_t FirstDataPosition;
	uint32_t FileSize;
	uint32_t TransferredData;
}FAT_SystemInfo_t;

typedef struct /* FAT Base Entry Metadata */
{
	uint8_t Name[11];
	uint8_t Attribute;
	uint8_t Reserved;
	uint8_t CreationMS;
	uint8_t CreationTime[2];
	uint8_t CreationDate[2];
	uint8_t LastAccessDate[2];
	uint8_t FirstClusterHI[2];
	uint8_t LastWriteTime[2];
	uint8_t LastWriteDate[2];
	uint8_t FirstClusterLO[2];
	uint8_t Size[4];
}FAT_BaseEntry_t;

/* Exported Variables */
extern uint8_t ramBuffer[];
extern uint8_t blkSector[];
extern FAT_BootSector_t *fatFS;
extern FAT_BaseEntry_t *binFile;
extern FAT_SystemInfo_t fatINFO;

/* Exported Function Prototypes */
void FAT_Init(void);

#endif /* INC_FAT_APP_H_ */
