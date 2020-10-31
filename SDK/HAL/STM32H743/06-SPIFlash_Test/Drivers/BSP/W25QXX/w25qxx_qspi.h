#ifndef __w25qxx_H
#define __w25qxx_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"

typedef enum
{
  w25qxx_OK       = 0x00,
  w25qxx_ERROR    = 0x01,
  w25qxx_BUSY     = 0x02,
  w25qxx_TIMEOUT  = 0x03,
	
	w25qxx_QPIMode = 0x04,
	w25qxx_SPIMode = 0x05,
	
	w25qxx_DTRMode = 0x06,
	w25qxx_NormalMode = 0x07,
	
} w25qxx_StatusTypeDef;

/* =============== W25Qxx CMD ================ */
#define W25X_WriteEnable         0x06 
#define W25X_WriteDisable        0x04 
#define W25X_ReadStatusReg1      0x05 
#define W25X_ReadStatusReg2		   0x35 
#define W25X_ReadStatusReg3		   0x15 
#define W25X_WriteStatusReg1     0x01 
#define W25X_WriteStatusReg2     0x31 
#define W25X_WriteStatusReg3     0x11 
#define W25X_ReadData			       0x03 
#define W25X_FastReadData		     0x0B 
#define W25X_FastReadDual		     0x3B 
#define W25X_PageProgram		     0x02 
#define W25X_BlockErase			     0xD8 
#define W25X_SectorErase		     0x20 
#define W25X_ChipErase			     0xC7 
#define W25X_PowerDown			     0xB9 
#define W25X_ReleasePowerDown	   0xAB 
#define W25X_DeviceID			       0xAB 
#define W25X_ManufactDeviceID	   0x90 
#define W25X_JedecDeviceID		   0x9F 
#define W25X_Enable4ByteAddr     0xB7
#define W25X_Exit4ByteAddr       0xE9
#define W25X_SetReadParam		     0xC0 
#define W25X_EnterQSPIMode       0x38
#define W25X_ExitQSPIMode        0xFF

#define W25X_EnableReset         0x66
#define W25X_ResetDevice         0x99

#define W25X_QUAD_INOUT_FAST_READ_CMD             0xEB
#define W25X_QUAD_INOUT_FAST_READ_DTR_CMD         0xED
#define W25X_QUAD_INOUT_FAST_READ_4_BYTE_ADDR_CMD 0xEC
#define W25X_QUAD_INOUT_FAST_READ_4_BYTE_DTR_CMD  0xEE
#define W25X_QUAD_ManufactDeviceID                0x94
#define W25X_QUAD_INPUT_PAGE_PROG_CMD             0x32   /*!< Page Program 3 Byte Address */

/* 4-byte Address Mode Operations */
#define W25X_ENTER_4_BYTE_ADDR_MODE_CMD           0xB7
#define W25X_EXIT_4_BYTE_ADDR_MODE_CMD            0xE9

/* Dummy cycles for DTR read mode */
#define W25X_DUMMY_CYCLES_READ_QUAD_DTR  4U
#define W25X_DUMMY_CYCLES_READ_QUAD      6U



/**
  * @brief  W25Qxx Registers
  */
/* Status Register */
#define W25X_SR_WIP              (0x01)    /*!< Write in progress */
#define W25X_SR_WREN             (0x02)    /*!< Write enable latch */

void w25qxx_Init(void);
uint16_t w25qxx_GetID(void);
uint8_t w25qxx_ReadAllStatusReg(void);
uint8_t w25qxx_ReadSR(uint8_t SR);
uint8_t w25qxx_WriteSR(uint8_t SR,uint8_t data);
uint8_t w25qxx_SetReadParameters(uint8_t DummyClock,uint8_t WrapLenth);
uint8_t w25qxx_EnterQPI(void);
uint8_t w25qxx_Startup(uint8_t DTRMode);
uint8_t W25qxx_WriteEnable(void);
uint8_t W25qxx_EraseSector(uint32_t SectorAddress);
uint8_t W25qxx_EraseBlock(uint32_t BlockAddress);
uint8_t W25qxx_EraseChip(void);
uint8_t W25qxx_PageProgram(uint8_t *pData, uint32_t WriteAddr, uint32_t Size);
uint8_t W25qxx_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size);
void W25qxx_WriteNoCheck(uint8_t *pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite);
void W25qxx_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);

#ifdef __cplusplus
}
#endif

#endif
