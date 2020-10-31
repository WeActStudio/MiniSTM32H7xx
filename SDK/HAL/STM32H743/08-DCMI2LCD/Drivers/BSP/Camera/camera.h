#ifndef CAMERA_H
#define CAMERA_H

#include "main.h"

#define XCLK_TIM       0
#define XCLK_MCO       1

#define OV7670_ADDRESS 0x42
#define OV2640_ADDRESS 0x60
#define OV7725_ADDRESS 0x42
#define OV5640_ADDRESS 0x78

#define Camera_OK 0
#define camera_ERROR 1
#define Camera_delay HAL_Delay

struct regval_t{
	uint8_t reg_addr;
	uint8_t value;
};

typedef enum {
	PIXFORMAT_INVALID = 0,
	PIXFORMAT_RGB565,    // 2BPP/RGB565
	PIXFORMAT_JPEG,      // JPEG/COMPRESSED
	PIXFORMAT_YUV422,
	PIXFORMAT_GRAYSCALE,
	PIXFORMAT_BAYER,
} pixformat_t;

typedef enum {
    FRAMESIZE_INVALID = 0,
    // C/SIF Resolutions
    FRAMESIZE_QQCIF,    // 88x72 x
    FRAMESIZE_QCIF,     // 176x144
    FRAMESIZE_CIF,      // 352x288 x
    FRAMESIZE_QQSIF,    // 88x60 x
    FRAMESIZE_QSIF,     // 176x120
    FRAMESIZE_SIF,      // 352x240 x
    // VGA Resolutions
    FRAMESIZE_QQQQVGA,  // 40x30 x
    FRAMESIZE_QQQVGA,   // 80x60 x
    FRAMESIZE_QQVGA,    // 160x120 x
    FRAMESIZE_QVGA,     // 320x240 *
    FRAMESIZE_VGA,      // 640x480 *
    FRAMESIZE_HQQQVGA,  // 60x40
    FRAMESIZE_HQQVGA,   // 120x80 x
    FRAMESIZE_HQVGA,    // 240x160
    FRAMESIZE_HVGA,     // 480x320
    // FFT Resolutions
    FRAMESIZE_64X32,    // 64x32 x
    FRAMESIZE_64X64,    // 64x64 x
    FRAMESIZE_128X64,   // 128x64 x
    FRAMESIZE_128X128,  // 128x128
    // Other
    FRAMESIZE_LCD,      // 128x160
    FRAMESIZE_QQVGA2,   // 128x160
    FRAMESIZE_WVGA,     // 720x480 x
    FRAMESIZE_WVGA2,    // 752x480 x
    FRAMESIZE_SVGA,     // 800x600 x
    FRAMESIZE_XGA,      // 1024x768 x
    FRAMESIZE_SXGA,     // 1280x1024
    FRAMESIZE_UXGA,     // 1600x1200
    FRAMESIZE_720P,     // 1280x720 *
    FRAMESIZE_1080P,    // 1920x1080 *
    FRAMESIZE_960P,     // 1280x960 *
    FRAMESIZE_5MPP,     // 2592x1944 *
} framesize_t;

typedef struct {
	I2C_HandleTypeDef *hi2c;
	uint8_t addr;
	uint32_t timeout;
	uint16_t manuf_id;
	uint16_t device_id;
	framesize_t framesize;
	pixformat_t pixformat;
} Camera_HandleTypeDef;

extern Camera_HandleTypeDef hcamera;;
extern const uint16_t dvp_cam_resolution[][2];

int32_t Camera_WriteReg(Camera_HandleTypeDef *hov, uint8_t regAddr, const uint8_t *pData);
int32_t Camera_ReadReg(Camera_HandleTypeDef *hov, uint8_t regAddr, uint8_t *pData);
int32_t Camera_WriteRegb2(Camera_HandleTypeDef *hov, uint16_t reg_addr, uint8_t reg_data);
int32_t Camera_ReadRegb2(Camera_HandleTypeDef *hov, uint16_t reg_addr, uint8_t *reg_data);
int32_t Camera_WriteRegList(Camera_HandleTypeDef *hov, const struct regval_t *reg_list);
int32_t Camera_read_id(Camera_HandleTypeDef *hov);
void Camera_Reset(Camera_HandleTypeDef *hov);
void Camera_XCLK_Set(uint8_t xclktype);
void Camera_Init_Device(I2C_HandleTypeDef *hi2c, framesize_t framesize);

#endif


