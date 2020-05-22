/*
 * usbd_camera.h
 *
 */

#ifndef INC_USBD_CAMERA_H_
#define INC_USBD_CAMERA_H_

#include "main.h"
#include "usbd_uvc.h"
//#include "ov7670.h"

extern USBD_UVC_CameraTypeDef USBD_Camera;

void Camera_Init(I2C_HandleTypeDef *hi2c);
void Camera_Loop(void);

#define CAMERA_BUFFER_STATE_FREE 1
#define CAMERA_BUFFER_STATE_LOCKED_BY_DMA 2
#define CAMERA_BUFFER_STATE_READY 3
#define CAMERA_BUFFER_STATE_LOCKED_BY_UVC 4

#endif /* INC_USBD_CAMERA_H_ */
