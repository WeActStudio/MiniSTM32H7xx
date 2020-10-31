#include "w25qxx_qspi.h"
#include "quadspi.h"

static uint32_t QSPI_EnableMemoryMappedMode(QSPI_HandleTypeDef *hqspi,uint8_t DTRMode);
static uint32_t QSPI_ResetDevice(QSPI_HandleTypeDef *hqspi);
static uint8_t QSPI_EnterQPI(QSPI_HandleTypeDef *hqspi);
static uint32_t QSPI_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi, uint32_t Timeout);
static uint32_t QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi);
static uint32_t QSPI_EnterFourBytesAddress(QSPI_HandleTypeDef *hqspi);

static uint8_t QSPI_Send_CMD(QSPI_HandleTypeDef *hqspi,uint32_t instruction, uint32_t address,uint32_t addressSize,uint32_t dummyCycles, 
                    uint32_t instructionMode,uint32_t addressMode, uint32_t dataMode, uint32_t dataSize);
										
w25qxx_StatusTypeDef w25qxx_Mode = w25qxx_SPIMode;
uint8_t w25qxx_StatusReg[3];
uint16_t w25qxx_ID;

void w25qxx_Init(void)
{
	HAL_Delay(5);
	MX_QUADSPI_Init();
	QSPI_ResetDevice(&hqspi);
	HAL_Delay(0); // 1ms wait device stable
	w25qxx_ID = w25qxx_GetID();
	w25qxx_ReadAllStatusReg();
}

uint16_t w25qxx_GetID(void)
{
	uint8_t ID[6];
	uint16_t deviceID;
	
	if(w25qxx_Mode == w25qxx_SPIMode)
		QSPI_Send_CMD(&hqspi,W25X_QUAD_ManufactDeviceID,0x00,QSPI_ADDRESS_24_BITS,6,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_4_LINES, QSPI_DATA_4_LINES, sizeof(ID));
	else
		QSPI_Send_CMD(&hqspi,W25X_ManufactDeviceID,0x00,QSPI_ADDRESS_24_BITS,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES, QSPI_DATA_4_LINES, sizeof(ID));

	/* Reception of the data */
  if (HAL_QSPI_Receive(&hqspi, ID, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return w25qxx_ERROR;
  }
	deviceID = (ID[0] << 8) | ID[1];

	return deviceID;
}

uint8_t w25qxx_ReadSR(uint8_t SR)
{
	uint8_t byte=0;
	if(w25qxx_Mode == w25qxx_SPIMode)
		QSPI_Send_CMD(&hqspi,SR,0x00,QSPI_ADDRESS_8_BITS,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE, QSPI_DATA_1_LINE, 1);
	else
		QSPI_Send_CMD(&hqspi,SR,0x00,QSPI_ADDRESS_8_BITS,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE, QSPI_DATA_4_LINES, 1);
	
	if (HAL_QSPI_Receive(&hqspi,&byte,HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
	{
		
	}
  return byte;
}

uint8_t w25qxx_WriteSR(uint8_t SR,uint8_t data)
{
	if(w25qxx_Mode == w25qxx_SPIMode)
		QSPI_Send_CMD(&hqspi,SR,0x00,QSPI_ADDRESS_8_BITS,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE, QSPI_DATA_1_LINE, 1);
	else
		QSPI_Send_CMD(&hqspi,SR,0x00,QSPI_ADDRESS_8_BITS,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE, QSPI_DATA_4_LINES, 1);

  return HAL_QSPI_Transmit(&hqspi,&data,HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
}

uint8_t w25qxx_ReadAllStatusReg(void)
{
	
	w25qxx_StatusReg[0] = w25qxx_ReadSR(W25X_ReadStatusReg1);
	w25qxx_StatusReg[1] = w25qxx_ReadSR(W25X_ReadStatusReg2);
	w25qxx_StatusReg[2] = w25qxx_ReadSR(W25X_ReadStatusReg3);
	return w25qxx_OK;
}

//等待空闲
void W25QXX_Wait_Busy(void)
{
	while((w25qxx_ReadSR(W25X_ReadStatusReg1) & 0x01) == 0x01);
}

// Only use in QPI mode
uint8_t w25qxx_SetReadParameters(uint8_t DummyClock,uint8_t WrapLenth)
{
	uint8_t send;
	send = (DummyClock/2 -1)<<4 | ((WrapLenth/8 - 1)&0x03);
	
	W25qxx_WriteEnable();
	
	QSPI_Send_CMD(&hqspi,W25X_SetReadParam,0x00,QSPI_ADDRESS_8_BITS,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE, QSPI_DATA_4_LINES, 1);
	
	return HAL_QSPI_Transmit(&hqspi,&send,HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
}

uint8_t w25qxx_EnterQPI(void)
{
	/* Enter QSPI memory in QSPI mode */
  if(QSPI_EnterQPI(&hqspi) != w25qxx_OK)
  {
    return w25qxx_ERROR;
  }
	
	return w25qxx_SetReadParameters(8,8);
}


/**
  * @brief  Initializes and configure the QSPI interface.
  * @retval QSPI memory status
  */
uint8_t w25qxx_Startup(uint8_t DTRMode)
{
  /* Enable MemoryMapped mode */
  if( QSPI_EnableMemoryMappedMode(&hqspi,DTRMode) != w25qxx_OK )
  {
    return w25qxx_ERROR;
  }
  return w25qxx_OK;
}

uint8_t W25qxx_WriteEnable(void)
{
	return QSPI_WriteEnable(&hqspi);
}
/**
  * @brief  Erase 4KB Sector of the OSPI memory.
	* @param  SectorAddress: Sector address to erase
  * @retval QSPI memory status
  */
uint8_t W25qxx_EraseSector(uint32_t SectorAddress)
{
	uint8_t result;
	
	W25qxx_WriteEnable();
	W25QXX_Wait_Busy();
	
	if(w25qxx_Mode == w25qxx_SPIMode)
		result = QSPI_Send_CMD(&hqspi,W25X_SectorErase,SectorAddress,QSPI_ADDRESS_24_BITS,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_DATA_NONE,0);
  else
		result = QSPI_Send_CMD(&hqspi,W25X_SectorErase,SectorAddress,QSPI_ADDRESS_24_BITS,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_DATA_NONE,0);
	
	/* 等待擦除完成 */
	if(result == w25qxx_OK)
		W25QXX_Wait_Busy();

	return result;
}

/**
  * @brief  Erase 64KB Sector of the OSPI memory.
	* @param  SectorAddress: Sector address to erase
  * @retval QSPI memory status
  */
uint8_t W25qxx_EraseBlock(uint32_t BlockAddress)
{
	uint8_t result;
	
	W25qxx_WriteEnable();
	W25QXX_Wait_Busy();
	
	if(w25qxx_Mode == w25qxx_SPIMode)
		result = QSPI_Send_CMD(&hqspi,W25X_BlockErase,BlockAddress,QSPI_ADDRESS_24_BITS,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_DATA_NONE,0);
  else
		result = QSPI_Send_CMD(&hqspi,W25X_BlockErase,BlockAddress,QSPI_ADDRESS_24_BITS,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_DATA_NONE,0);
	
	/* 等待擦除完成 */
	if(result == w25qxx_OK)
		W25QXX_Wait_Busy();
	
	return result;
}

/**
  * @brief  Whole chip erase.
	* @param  SectorAddress: Sector address to erase
  * @retval QSPI memory status
  */
uint8_t W25qxx_EraseChip(void)
{
	uint8_t result;
	
	W25qxx_WriteEnable();
	W25QXX_Wait_Busy();
	
	if(w25qxx_Mode == w25qxx_SPIMode)
		result = QSPI_Send_CMD(&hqspi,W25X_ChipErase,0x00,QSPI_ADDRESS_8_BITS,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_DATA_NONE,0);
  else
		result = QSPI_Send_CMD(&hqspi,W25X_ChipErase,0x00,QSPI_ADDRESS_8_BITS,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_NONE,QSPI_DATA_NONE,0);
	
	/* 等待擦除完成 */
	if(result == w25qxx_OK)
		W25QXX_Wait_Busy();
	
	return result;
}

/**
  * @brief  Writes an amount of data to the OSPI memory.
  * @param  pData Pointer to data to be written
  * @param  WriteAddr Write start address
  * @param  Size Size of data to write. Range 1 ~ W25qxx page size
  * @retval QSPI memory status
  */
uint8_t W25qxx_PageProgram(uint8_t *pData, uint32_t WriteAddr, uint32_t Size)
{
	uint8_t result;
	
	W25qxx_WriteEnable();
	
	if(w25qxx_Mode == w25qxx_SPIMode)
		result = QSPI_Send_CMD(&hqspi,W25X_QUAD_INPUT_PAGE_PROG_CMD,WriteAddr,QSPI_ADDRESS_24_BITS,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_1_LINE,QSPI_DATA_4_LINES,Size);
  else
		result = QSPI_Send_CMD(&hqspi,W25X_PageProgram,WriteAddr,QSPI_ADDRESS_24_BITS,0,QSPI_INSTRUCTION_4_LINES,QSPI_ADDRESS_4_LINES,QSPI_DATA_4_LINES,Size);
	
	if(result == w25qxx_OK)
		result = HAL_QSPI_Transmit(&hqspi,pData,HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
	
	/* 等待写入完成 */
	if(result == w25qxx_OK)
		W25QXX_Wait_Busy();
	
  return result;
}

//读取SPI FLASH,仅支持QPI模式
//在指定地址开始读取指定长度的数据
//pBuffer:数据存储区
//ReadAddr:开始读取的地址(最大32bit)
//NumByteToRead:要读取的字节数(最大65535)
uint8_t W25qxx_Read(uint8_t *pData, uint32_t ReadAddr, uint32_t Size)
{
	uint8_t result;
	
	QSPI_CommandTypeDef      s_command;

	/* Configure the command for the read instruction */
	
	if(w25qxx_Mode == w25qxx_QPIMode)
	{
		s_command.Instruction     = W25X_QUAD_INOUT_FAST_READ_CMD;
		s_command.InstructionMode = QSPI_INSTRUCTION_4_LINES;
	  s_command.DummyCycles     = W25X_DUMMY_CYCLES_READ_QUAD;
	}
	else 
	{
		s_command.Instruction     = W25X_QUAD_INOUT_FAST_READ_CMD;
		s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
		s_command.DummyCycles     = W25X_DUMMY_CYCLES_READ_QUAD-2;
  }
	
	s_command.Address           = ReadAddr;
	s_command.AddressMode       = QSPI_ADDRESS_4_LINES;
	s_command.AddressSize       = QSPI_ADDRESS_24_BITS;

	s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
	s_command.AlternateBytes    = 0xFF;
	s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;

	s_command.DataMode          = QSPI_DATA_4_LINES;	
	s_command.NbData            = Size;
		
	s_command.DdrMode         = QSPI_DDR_MODE_DISABLE;

	s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
	s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
	
	result = HAL_QSPI_Command(&hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
	
	if(result == w25qxx_OK)
		result = HAL_QSPI_Receive(&hqspi,pData,HAL_QPSI_TIMEOUT_DEFAULT_VALUE);
	
	return result;
}

//无检验写SPI FLASH
//必须确保所写的地址范围内的数据全部为0XFF,否则在非0XFF处写入的数据将失败!
//具有自动换页功能
//在指定地址开始写入指定长度的数据,但是要确保地址不越界!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(最大32bit)
//NumByteToWrite:要写入的字节数(最大65535)
//CHECK OK
void W25qxx_WriteNoCheck(uint8_t *pBuffer,uint32_t WriteAddr,uint32_t NumByteToWrite)
{
	uint16_t pageremain;	   
	pageremain = 256 - WriteAddr % 256; //单页剩余的字节数
	if (NumByteToWrite <= pageremain)
	{
		pageremain = NumByteToWrite; //不大于256个字节
	}
	while(1)
	{
		W25qxx_PageProgram(pBuffer, WriteAddr, pageremain);
		if (NumByteToWrite == pageremain)
		{
			break; //写入结束了
		}
	 	else //NumByteToWrite>pageremain
		{
			pBuffer += pageremain;
			WriteAddr += pageremain;

			NumByteToWrite -= pageremain; //减去已经写入了的字节数
			if (NumByteToWrite > 256)
				pageremain = 256; //一次可以写入256个字节
			else
				pageremain = NumByteToWrite; //不够256个字节了
		}
	}
}

//写SPI FLASH
//在指定地址开始写入指定长度的数据
//该函数带擦除操作!
//pBuffer:数据存储区
//WriteAddr:开始写入的地址(最大32bit)
//NumByteToWrite:要写入的字节数(最大65535)
void W25qxx_Write(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
	uint32_t secpos;
	uint16_t secoff;
	uint16_t secremain;
 	uint16_t i;
	uint8_t W25QXX_BUF[4096];

 	secpos = WriteAddr / 4096; //扇区地址
	secoff = WriteAddr % 4096; //在扇区内的偏移
	secremain = 4096 - secoff; //扇区剩余空间大小

 	if (NumByteToWrite <= secremain) secremain = NumByteToWrite; //不大于4096个字节
	while(1)
	{
		W25qxx_Read(W25QXX_BUF, secpos * 4096, 4096); //读出整个扇区的内容
		for (i = 0;i < secremain; i++) //校验数据
		{
			if (W25QXX_BUF[secoff+i] != 0XFF) break; //需要擦除
		}
		if (i < secremain) //需要擦除
		{
			W25qxx_EraseSector(secpos); //擦除这个扇区
			for (i = 0; i < secremain; i++) //复制
			{
				W25QXX_BUF[i + secoff] = pBuffer[i];
			}
			W25qxx_WriteNoCheck(W25QXX_BUF, secpos * 4096, 4096); //写入整个扇区
		}
		else
		{
			W25qxx_WriteNoCheck(pBuffer, WriteAddr, secremain); //写已经擦除了的,直接写入扇区剩余区间.
		}
		if (NumByteToWrite == secremain)
		{
			break; //写入结束了
		}
		else//写入未结束
		{
			secpos++; //扇区地址增1
			secoff = 0; //偏移位置为0

			pBuffer += secremain;  //指针偏移
			WriteAddr += secremain;//写地址偏移
			NumByteToWrite -= secremain; //字节数递减
			if (NumByteToWrite > 4096)
				secremain = 4096; //下一个扇区还是写不完
			else
				secremain = NumByteToWrite; //下一个扇区可以写完了
		}
	}
}

/**
  * @brief  Configure the QSPI in memory-mapped mode   QPI/SPI && DTR(DDR)/Normal Mode
	* @param  hqspi: QSPI handle
  * @param  DTRMode: w25qxx_DTRMode DTR mode ,w25qxx_NormalMode Normal mode
  * @retval QSPI memory status
  */
static uint32_t QSPI_EnableMemoryMappedMode(QSPI_HandleTypeDef *hqspi,uint8_t DTRMode)
{
  QSPI_CommandTypeDef      s_command;
  QSPI_MemoryMappedTypeDef s_mem_mapped_cfg;

  /* Configure the command for the read instruction */
	if(w25qxx_Mode == w25qxx_QPIMode)
		s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
	else 
		s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	
  s_command.AddressMode       = QSPI_ADDRESS_4_LINES;
	s_command.Address           = 0;
  s_command.AddressSize       = QSPI_ADDRESS_24_BITS;
	
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_4_LINES;
	s_command.AlternateBytes    = 0xEF;
	s_command.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;

  s_command.DataMode          = QSPI_DATA_4_LINES;	
	
	if(DTRMode == w25qxx_DTRMode)
	{
		s_command.Instruction     = W25X_QUAD_INOUT_FAST_READ_DTR_CMD; 
		s_command.DummyCycles     = W25X_DUMMY_CYCLES_READ_QUAD_DTR;
		s_command.DdrMode         = QSPI_DDR_MODE_ENABLE;
	}
	else
	{
		s_command.Instruction     = W25X_QUAD_INOUT_FAST_READ_CMD;
		
		if(w25qxx_Mode == w25qxx_QPIMode)
			s_command.DummyCycles   = W25X_DUMMY_CYCLES_READ_QUAD;
		else
			s_command.DummyCycles   = W25X_DUMMY_CYCLES_READ_QUAD-2;
		
		s_command.DdrMode         = QSPI_DDR_MODE_DISABLE;
	}
	
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_ONLY_FIRST_CMD;

  /* Configure the memory mapped mode */
  s_mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
  s_mem_mapped_cfg.TimeOutPeriod     = 0;

  if (HAL_QSPI_MemoryMapped(hqspi, &s_command, &s_mem_mapped_cfg) != HAL_OK)
  {
    return w25qxx_ERROR;
  }

  return w25qxx_OK;
}

/**
  * @brief  This function reset the QSPI memory.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint32_t QSPI_ResetDevice(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef s_command;

  /* Initialize the reset enable command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = W25X_EnableReset;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return w25qxx_ERROR;
  }

  /* Send the reset device command */
  s_command.Instruction = W25X_ResetDevice;
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return w25qxx_ERROR;
  }

  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = W25X_EnableReset;
  /* Send the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return w25qxx_ERROR;
  }

  /* Send the reset memory command */
  s_command.Instruction = W25X_ResetDevice;
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return w25qxx_ERROR;
  }
	
  return w25qxx_OK;
}

/**
 * @brief	QSPI发送命令
 *
 * @param   instruction		要发送的指令
 * @param   address			发送到的目的地址
 * @param   addressSize	发送到的目的地址大小
 * @param   dummyCycles		空指令周期数
 * @param   instructionMode		指令模式;
 * @param   addressMode		地址模式; QSPI_ADDRESS_NONE,QSPI_ADDRESS_1_LINE,QSPI_ADDRESS_2_LINES,QSPI_ADDRESS_4_LINES
 * @param   dataMode		数据模式; QSPI_DATA_NONE,QSPI_DATA_1_LINE,QSPI_DATA_2_LINES,QSPI_DATA_4_LINES
 * @param   dataSize        待传输的数据长度
 *
 * @return  uint8_t			w25qxx_OK:正常
 *                      w25qxx_ERROR:错误
 */
static uint8_t QSPI_Send_CMD(QSPI_HandleTypeDef *hqspi,uint32_t instruction, uint32_t address,uint32_t addressSize,uint32_t dummyCycles, 
                    uint32_t instructionMode,uint32_t addressMode, uint32_t dataMode, uint32_t dataSize)
{
    QSPI_CommandTypeDef Cmdhandler;

    Cmdhandler.Instruction        = instruction;   
	  Cmdhandler.InstructionMode    = instructionMode;  
	
    Cmdhandler.Address            = address;
    Cmdhandler.AddressSize        = addressSize;
	  Cmdhandler.AddressMode        = addressMode;
	  
	  Cmdhandler.AlternateBytes     = 0x00;
    Cmdhandler.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
	  Cmdhandler.AlternateByteMode  = QSPI_ALTERNATE_BYTES_NONE;                              
    Cmdhandler.DummyCycles        = dummyCycles;                   
       					      				
    Cmdhandler.DataMode           = dataMode;
    Cmdhandler.NbData             = dataSize; 
	
    Cmdhandler.DdrMode            = QSPI_DDR_MODE_DISABLE;           	
    Cmdhandler.DdrHoldHalfCycle   = QSPI_DDR_HHC_ANALOG_DELAY;
    Cmdhandler.SIOOMode           = QSPI_SIOO_INST_EVERY_CMD;

    if(HAL_QSPI_Command(hqspi, &Cmdhandler, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
      return w25qxx_ERROR;

    return w25qxx_OK;
}

/**
  * @brief  This function set the QSPI memory in 4-byte address mode
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint32_t QSPI_EnterFourBytesAddress(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef s_command;

  /* Initialize the command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = W25X_Enable4ByteAddr;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Enable write operations */
  if (QSPI_WriteEnable(hqspi) != w25qxx_OK)
  {
    return w25qxx_ERROR;
  }

  /* Send the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return w25qxx_ERROR;
  }

  /* Configure automatic polling mode to wait the memory is ready */
  if (QSPI_AutoPollingMemReady(hqspi, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != w25qxx_OK)
  {
    return w25qxx_ERROR;
  }

  return w25qxx_OK;
}

/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint32_t QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef     s_command;
  QSPI_AutoPollingTypeDef s_config;

  /* Enable write operations */
	if(w25qxx_Mode == w25qxx_QPIMode)
		s_command.InstructionMode = QSPI_INSTRUCTION_4_LINES;
	else 
		s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;

  s_command.Instruction       = W25X_WriteEnable;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return w25qxx_ERROR;
  }

  /* Configure automatic polling mode to wait for write enabling */
  s_config.Match           = W25X_SR_WREN;
  s_config.Mask            = W25X_SR_WREN;
  s_config.MatchMode       = QSPI_MATCH_MODE_AND;
  s_config.StatusBytesSize = 1;
  s_config.Interval        = 0x10;
  s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  s_command.Instruction    = W25X_ReadStatusReg1;
	
	if(w25qxx_Mode == w25qxx_QPIMode)
		s_command.DataMode     = QSPI_DATA_4_LINES;
  else 
		s_command.DataMode     = QSPI_DATA_1_LINE;
	
  if (HAL_QSPI_AutoPolling(hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return w25qxx_ERROR;
  }

  return w25qxx_OK;
}

/**
  * @brief  This function read the SR of the memory and wait the EOP.
  * @param  hqspi: QSPI handle
  * @param  Timeout
  * @retval None
  */
static uint32_t QSPI_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi, uint32_t Timeout)
{
  QSPI_CommandTypeDef     s_command;
  QSPI_AutoPollingTypeDef s_config;

  /* Configure automatic polling mode to wait for memory ready */
	
	if(w25qxx_Mode == w25qxx_SPIMode)
		s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
	else
		s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
	
  s_command.Instruction       = W25X_ReadStatusReg1;
	
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
	s_command.Address           = 0x00;
	s_command.AddressSize       = QSPI_ADDRESS_8_BITS;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
	
	if(w25qxx_Mode == w25qxx_SPIMode)
		s_command.DataMode        = QSPI_DATA_1_LINE;
  else
		s_command.DataMode        = QSPI_DATA_4_LINES;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  s_config.Match           = 0;
	s_config.Mask            = W25X_SR_WIP;
  s_config.MatchMode       = QSPI_MATCH_MODE_AND;
  s_config.Interval        = 0x10;
  s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;
  s_config.StatusBytesSize = 1;
  
  return HAL_QSPI_AutoPolling(hqspi, &s_command, &s_config, Timeout);

}

/**
  * @brief  This function enter the QSPI memory in QPI mode
  * @param  hqspi QSPI handle 
  * @retval QSPI status
  */
static uint8_t QSPI_EnterQPI(QSPI_HandleTypeDef *hqspi)
{
	uint8_t stareg2;
	stareg2 = w25qxx_ReadSR(W25X_ReadStatusReg2);
	if((stareg2 & 0X02) == 0) //QE位未使能
	{
		W25qxx_WriteEnable();
		stareg2 |= 1<<1; //使能QE位
		w25qxx_WriteSR(W25X_WriteStatusReg2,stareg2);
	}
	QSPI_Send_CMD(hqspi,W25X_EnterQSPIMode,0x00,QSPI_ADDRESS_8_BITS,0,QSPI_INSTRUCTION_1_LINE,QSPI_ADDRESS_NONE,QSPI_DATA_NONE,0);
  
	/* Configure automatic polling mode to wait the memory is ready */
  if (QSPI_AutoPollingMemReady(hqspi, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != w25qxx_OK)
  {
    return w25qxx_ERROR;
  }
	
  w25qxx_Mode = w25qxx_QPIMode;
	
  return w25qxx_OK;
}
