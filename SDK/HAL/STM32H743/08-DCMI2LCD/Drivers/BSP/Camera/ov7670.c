#include "ov7670.h"
#include "ov7670_regs.h"

int OV7670_WriteReg(uint8_t regAddr, const uint8_t *pData)
{
	uint8_t tt[2];
	tt[0] = regAddr;
	tt[1] = pData[0];
	if (HAL_I2C_Master_Transmit(hcamera.hi2c, hcamera.addr,tt,2,hcamera.timeout) == HAL_OK)
	{
		return OV7670_OK;
	}
	else
	{
		return OV7670_ERROR;
	}
}

int OV7670_ReadReg(uint8_t regAddr, uint8_t *pData)
{
	HAL_I2C_Master_Transmit(hcamera.hi2c, hcamera.addr+1,&regAddr,1,hcamera.timeout);
	if (HAL_I2C_Master_Receive(hcamera.hi2c, hcamera.addr+1,pData,1,hcamera.timeout) == HAL_OK)
	{
		return OV7670_OK;
	}
	else
	{
		return OV7670_ERROR;
	}
}

int OV7670_Reset(void)
{
	HAL_Delay(100);
	uint8_t data = COM7_RESET;
	if (OV7670_WriteReg(REG_COM7, &data) != OV7670_OK)
	{
		return OV7670_ERROR;
	}
	HAL_Delay(100);
	return OV7670_OK;
}

int OV7670_WriteRegList(const struct regval_t *reg_list)
{
	const struct regval_t *pReg = reg_list;
	while (pReg->reg_addr != 0xFF && pReg->value != 0xFF)
	{
		int write_result = OV7670_WriteReg( pReg->reg_addr, &(pReg->value));
		if (write_result != OV7670_OK)
		{
			return write_result;
		}
		pReg++;
	}
	return OV7670_OK;
}

////////////////////////////////////////////////////////////////////////////
//OV7670功能设置
//白平衡设置
//0:自动
//1:太阳sunny
//2,阴天cloudy
//3,办公室office
//4,家里home
void OV7670_Light_Mode(uint8_t mode)
{
	uint8_t reg13val=0XE7;//默认就是设置为自动白平衡
	uint8_t reg01val=0;
	uint8_t reg02val=0;
	switch(mode)
	{
		case 1://sunny
			reg13val=0XE5;
			reg01val=0X5A;
			reg02val=0X5C;
			break;	
		case 2://cloudy
			reg13val=0XE5;
			reg01val=0X58;
			reg02val=0X60;
			break;	
		case 3://office
			reg13val=0XE5;
			reg01val=0X84;
			reg02val=0X4c;
			break;	
		case 4://home
			reg13val=0XE5;
			reg01val=0X96;
			reg02val=0X40;
			break;	
	}
	OV7670_WriteReg(REG_COM8, &reg13val);//COM8设置 
	OV7670_WriteReg(REG_BLUE,&reg01val);//AWB蓝色通道增益 
	OV7670_WriteReg(REG_RED,&reg02val);//AWB红色通道增益 
}				

//色度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Color_Saturation(uint8_t sat)
{
	uint8_t reg4f5054val=0X80;//默认就是sat=2,即不调节色度的设置
 	uint8_t reg52val=0X22;
	uint8_t reg53val=0X5E;
	uint8_t reg54val=0X9E;
 	switch(sat)
	{
		case 0://-2
			reg4f5054val=0X40;  	 
			reg52val=0X11;
			reg53val=0X2F;	 	 
			break;	
		case 1://-1
			reg4f5054val=0X66;	    
			reg52val=0X1B;
			reg53val=0X4B;	  
			break;	
		case 3://1
			reg4f5054val=0X99;	   
			reg52val=0X28;
			reg53val=0X71;	   
			break;	
		case 4://2
			reg4f5054val=0XC0;	   
			reg52val=0X33;
			reg53val=0X8D;	   
			break;	
	}
	OV7670_WriteReg(MTX1,&reg4f5054val);	//色彩矩阵系数1
	OV7670_WriteReg(MTX2,&reg4f5054val);	//色彩矩阵系数2 
	OV7670_WriteReg(MTX3,0X00);			//色彩矩阵系数3  
	OV7670_WriteReg(MTX4,&reg52val);		//色彩矩阵系数4 
	OV7670_WriteReg(MTX5,&reg53val);		//色彩矩阵系数5 
	OV7670_WriteReg(MTX6,&reg4f5054val);	//色彩矩阵系数6  
	OV7670_WriteReg(MTXS,&reg54val);			//MTXS 
}

//亮度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Brightness(uint8_t bright)
{
	uint8_t reg55val=0X00;//默认就是bright=2
  	switch(bright)
	{
		case 0://-2
			reg55val=0XB0;	 	 
			break;	
		case 1://-1
			reg55val=0X98;	 	 
			break;	
		case 3://1
			reg55val=0X18;	 	 
			break;	
		case 4://2
			reg55val=0X30;	 	 
			break;	
	}
	OV7670_WriteReg(REG_BRIGHT,&reg55val);	//亮度调节 
}

//对比度设置
//0:-2
//1:-1
//2,0
//3,1
//4,2
void OV7670_Contrast(uint8_t contrast)
{
	uint8_t reg56val=0X40;//默认就是contrast=2
  	switch(contrast)
	{
		case 0://-2
			reg56val=0X30;	 	 
			break;	
		case 1://-1
			reg56val=0X38;	 	 
			break;	
		case 3://1
			reg56val=0X50;	 	 
			break;	
		case 4://2
			reg56val=0X60;	 	 
			break;	
	}
	OV7670_WriteReg(REG_CONTRAS,&reg56val);	//对比度调节 
}

//特效设置
//0:普通模式    
//1,负片
//2,黑白   
//3,偏红色
//4,偏绿色
//5,偏蓝色
//6,复古	    
void OV7670_Special_Effects(uint8_t eft)
{
	uint8_t reg3aval=0X04;//默认为普通模式
	uint8_t reg67val=0XC0;
	uint8_t reg68val=0X80;
	switch(eft)
	{
		case 1://负片
			reg3aval=0X24;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 2://黑白
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0X80;
			break;	
		case 3://偏红色
			reg3aval=0X14;
			reg67val=0Xc0;
			reg68val=0X80;
			break;	
		case 4://偏绿色
			reg3aval=0X14;
			reg67val=0X40;
			reg68val=0X40;
			break;	
		case 5://偏蓝色
			reg3aval=0X14;
			reg67val=0X80;
			reg68val=0XC0;
			break;	
		case 6://复古
			reg3aval=0X14;
			reg67val=0XA0;
			reg68val=0X40;
			break;	 
	}
	OV7670_WriteReg(REG_TSLB,&reg3aval);//TSLB设置 
	OV7670_WriteReg(0X68,&reg67val);//MANU,手动U值 
	OV7670_WriteReg(0X67,&reg68val);//MANV,手动V值 
}

//设置图像输出窗口
//对QVGA设置。
void ov7670_Window_Set(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{
	uint16_t endx;
	uint16_t endy;
	uint8_t temp; 
	endx=sx+width*2;	//V*2
 	endy=sy+height*2;
	if(endy>784) endy-=784;
	
	OV7670_ReadReg(0X03,&temp);				//读取Vref之前的值
	temp&=0XF0;
	temp|=((endx&0X03)<<2)|(sx&0X03);
	OV7670_WriteReg(0X03,&temp);				//设置Vref的start和end的最低2位
	sx = (sx >>2)&0xFF;
	endx = (endx >>2)&0xFF;
	OV7670_WriteReg(0X19,(uint8_t *)&sx);			//设置Vref的start高8位
	OV7670_WriteReg(0X1A,(uint8_t *)&endx);			//设置Vref的end的高8位

	OV7670_ReadReg(0X32,&temp);				//读取Href之前的值
	temp&=0XC0;
	temp|=((endy&0X07)<<3)|(sy&0X07);
	OV7670_WriteReg(0X32,&temp);
	sy = (sy>>3)&0xFF;
	OV7670_WriteReg(0X17,(uint8_t *)&sy);			//设置Href的start高8位
	endy = (endy>>3)&0xFF;
	OV7670_WriteReg(0X18,(uint8_t *)&endy);			//设置Href的end的高8位
}

int OV7670_Config(void)
{
	int ov_reset_result = OV7670_Reset();
	if (ov_reset_result != OV7670_OK)
	{
		return ov_reset_result;
	}
	int ov_read_reg_result = OV7670_ERROR;
	int ov_write_reg_result = OV7670_ERROR;

	ov_write_reg_result = OV7670_WriteRegList( ov7670_default_regs);
	if (ov_write_reg_result != OV7670_OK)
	{
		return ov_write_reg_result;
	}

	uint8_t ov_com3 = 0x04; // REG_COM3 enable scaling
	ov_write_reg_result = OV7670_WriteReg( REG_COM3, &ov_com3);
	if (ov_write_reg_result != OV7670_OK)
	{
		return ov_write_reg_result;
	}

	ov_write_reg_result = OV7670_WriteRegList( qqvga_ov7670);
	if (ov_write_reg_result != OV7670_OK)
	{
		return ov_write_reg_result;
	}

	ov_write_reg_result = OV7670_WriteRegList( rgb565_ov7670);
	if (ov_write_reg_result != OV7670_OK)
	{
		return ov_write_reg_result;
	}
	
	// configure PCLK to 24MHz, input clk 12MHz

	uint8_t ov_clk_rc = 0;
	ov_read_reg_result = OV7670_ReadReg( REG_CLKRC, &ov_clk_rc);
	if (ov_read_reg_result != OV7670_OK)
	{
		return ov_read_reg_result;
	}
	ov_clk_rc = (ov_clk_rc & 0x80) | 0x01; // to enable prescaler by 2
	ov_write_reg_result = OV7670_WriteReg( REG_CLKRC, &ov_clk_rc);
	if (ov_write_reg_result != OV7670_OK)
	{
		return ov_write_reg_result;
	}

	uint8_t ov_dblv = 0;
	ov_read_reg_result = OV7670_ReadReg( REG_DBLV, &ov_dblv);
	if (ov_read_reg_result != OV7670_OK)
	{
		return ov_read_reg_result;
	}
	ov_dblv = (ov_dblv & 0x3F) | DBLV_PLL4; // to enable PLL x4
	ov_write_reg_result = OV7670_WriteReg( REG_DBLV, &ov_dblv);
	if (ov_write_reg_result != OV7670_OK)
	{
		return ov_write_reg_result;
	}
	
	HAL_Delay(100);
	
//	OV7670_Light_Mode(4);
//	OV7670_Color_Saturation(2);
//	OV7670_Brightness(2);
//	OV7670_Contrast(2);
//	OV7670_Special_Effects(0);
//	ov7670_Window_Set(0,320,160,120);
	
	return OV7670_OK;
}

