#include "camera.h"
#include "ov7670.h"
#include "ov2640.h"
#include "ov7725.h"

Camera_HandleTypeDef hcamera;
// Resolution table
//----------------------------------------
const uint16_t dvp_cam_resolution[][2] = {
    {0,    0   },
    // C/SIF Resolutions
    {88,   72  },    /* QQCIF     */
    {176,  144 },    /* QCIF      */
    {352,  288 },    /* CIF       */
    {88,   60  },    /* QQSIF     */
    {176,  120 },    /* QSIF      */
    {352,  240 },    /* SIF       */
    // VGA Resolutions
    {40,   30  },    /* QQQQVGA   */
    {80,   60  },    /* QQQVGA    */
    {160,  120 },    /* QQVGA     */
    {320,  240 },    /* QVGA      */
    {640,  480 },    /* VGA       */
    {60,   40  },    /* HQQQVGA   */
    {120,  80  },    /* HQQVGA    */
    {240,  160 },    /* HQVGA     */
    {480,  320 },    /* HVGA      */
    // FFT Resolutions
    {64,   32  },    /* 64x32     */
    {64,   64  },    /* 64x64     */
    {128,  64  },    /* 128x64    */
    {128,  128 },    /* 128x64    */
    // Other
    {128,  160 },    /* LCD       */
    {128,  160 },    /* QQVGA2    */
    {720,  480 },    /* WVGA      */
    {752,  480 },    /* WVGA2     */
    {800,  600 },    /* SVGA      */
    {1024, 768 },    /* XGA       */
    {1280, 1024},    /* SXGA      */
    {1600, 1200},    /* UXGA      */
    {1280, 720},     /* 720P      */
    {1920, 1080},    /* 1080P     */
    {1280, 960},     /* 960P      */
    {2592, 1944},    /* 5MP       */
};

int32_t Camera_WriteReg(Camera_HandleTypeDef *hov, uint8_t regAddr, const uint8_t *pData)
{
	uint8_t tt[2];
	tt[0] = regAddr;
	tt[1] = pData[0];
	if (HAL_I2C_Master_Transmit(hov->hi2c, hov->addr,tt,2,hov->timeout) == HAL_OK)
	{
		return Camera_OK;
	}
	else
	{
		return camera_ERROR;
	}
}

int32_t Camera_ReadReg(Camera_HandleTypeDef *hov, uint8_t regAddr, uint8_t *pData)
{
	HAL_I2C_Master_Transmit(hov->hi2c, hov->addr+1,&regAddr,1,hov->timeout);
	if (HAL_I2C_Master_Receive(hov->hi2c, hov->addr+1,pData,1,hov->timeout) == HAL_OK)
	{
		return Camera_OK;
	}
	else
	{
		return camera_ERROR;
	}
}

int32_t Camera_WriteRegList(Camera_HandleTypeDef *hov, const struct regval_t *reg_list)
{
	const struct regval_t *pReg = reg_list;
	while (pReg->reg_addr != 0xFF && pReg->value != 0xFF)
	{
		int write_result = Camera_WriteReg(hov, pReg->reg_addr, &(pReg->value));
		if (write_result != Camera_OK)
		{
			return write_result;
		}
		pReg++;
	}
	return Camera_OK;
}

int32_t Camera_read_id(Camera_HandleTypeDef *hov)
{
	uint8_t temp[2];
	temp[0] = 0x01;
	Camera_WriteReg(hov,0xFF, temp);
	Camera_ReadReg(hov,0x1C,&temp[0]);
	Camera_ReadReg(hov,0x1D,&temp[1]);
	hov->manuf_id = ((uint16_t)temp[0] << 8) | temp[1];
	Camera_ReadReg(hov,0x0A,&temp[0]);
	Camera_ReadReg(hov,0x0B,&temp[1]);
  hov->device_id = ((uint16_t)temp[0] << 8) | temp[1];
  return 0;
}

void Camera_Reset(Camera_HandleTypeDef *hov)
{
	uint8_t temp;
	temp = 0x01;
	Camera_WriteReg(hov,0xFF, &temp);
	temp = 0x80;
	Camera_WriteReg(hov,0x12, &temp);
	HAL_Delay(100);
}

void Camera_Init_Device(I2C_HandleTypeDef *hi2c)
{
	hcamera.hi2c = hi2c;
	hcamera.addr = OV7670_ADDRESS;
	hcamera.timeout = 100;
  
	Camera_read_id(&hcamera);
	if(hcamera.manuf_id == 0x7fa2 && hcamera.device_id == 0x7673)
		OV7670_Config();	
	else
	{
		hcamera.addr = OV2640_ADDRESS;
		Camera_read_id(&hcamera);
		if(hcamera.manuf_id == 0x7fa2 && ((hcamera.device_id - 0x2641) <=2 ))
			ov2640_init();
		else
		{
			hcamera.addr = OV7725_ADDRESS;
			Camera_read_id(&hcamera);
			if(hcamera.manuf_id == 0x7fa2 && ((hcamera.device_id - 0x7721) <=2 ))
				ov7725_init();
			else
			{
				hcamera.addr = 0;
				hcamera.device_id = 0;
			}
		}
	}
}
