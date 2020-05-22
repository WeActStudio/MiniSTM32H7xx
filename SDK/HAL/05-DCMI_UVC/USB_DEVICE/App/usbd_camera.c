#include "usbd_camera.h"
#include "dcmi.h"
#include "camera.h"

uint16_t pic[160][120];

void Camera_On(void);
void Camera_Off(void);
uint8_t *Camera_GetFrame(uint32_t *pFrameLength);
void Camera_FreeFrame(uint8_t *frame);


USBD_UVC_CameraTypeDef USBD_Camera =
{
	Camera_On,
	Camera_Off,
	Camera_GetFrame,
	Camera_FreeFrame
};

uint8_t camerax;
void Camera_SignalError(void)
{

}
void Camera_Init(I2C_HandleTypeDef *hi2c)
{
  Camera_Init_Device(hi2c);
}

void Camera_Fill_Buffer(int bufferIndex)
{
	
}

void Camera_On(void)
{
	camerax = 1;
	HAL_DCMI_Start_DMA(&hdcmi,DCMI_MODE_CONTINUOUS,(uint32_t)&pic, 160 * 120 * 2 / 4);
}

void Camera_Off(void)
{
	HAL_DCMI_Stop(&hdcmi);
	
	camerax = 0;
}

uint8_t reg,data,write,read;
void Camera_Loop(void)
{
	if(write == 1)
	{
		write = 0;
		Camera_WriteReg(&hcamera,reg,&data);
	}
	else if(read == 1)
	{
		read = 0;
		Camera_ReadReg(&hcamera,reg,&data);
	}
}

uint32_t fps = 0;
uint8_t *Camera_GetFrame(uint32_t *pFrameLength)
{
	static uint32_t fps_o = 0,count = 0,tick = 0;
	count ++;
	if(HAL_GetTick()- tick >= 1000)
	{
		tick = HAL_GetTick();
		fps = count - fps_o;
		fps_o = count;
	}
	pFrameLength[0] = 160*120*2;
	
	return (uint8_t *)&pic;
}

void Camera_FreeFrame(uint8_t *frame)
{
	//sda ++;
}
