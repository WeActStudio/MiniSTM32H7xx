#ifndef OV7670_H
#define OV7670_H

#include "main.h"
#include "camera.h"

#define OV7670_OK 0
#define OV7670_ERROR 1

int OV7670_Reset(void);
int OV7670_WriteReg(uint8_t regAddr, const uint8_t *pData);
int OV7670_ReadReg( uint8_t regAddr, uint8_t *pData);
int OV7670_WriteRegList( const struct regval_t *reg_list);
int OV7670_Config(void);

#endif
