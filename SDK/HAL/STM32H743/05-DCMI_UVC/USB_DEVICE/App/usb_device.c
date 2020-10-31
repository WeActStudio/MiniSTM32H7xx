#include "usb_device.h"
#include "usbd_core.h"

/* USB Device Core handle declaration */
USBD_HandleTypeDef hUsbDeviceFS;

/* init function */
void USB_DEVICE_Init(void)
{
  /* Init Device Library,Add Supported Class and Start the library*/
  USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);

  USBD_RegisterClass(&hUsbDeviceFS, &USBD_UVC);

  USBD_UVC_RegisterCamera(&hUsbDeviceFS, &USBD_Camera);

  USBD_Start(&hUsbDeviceFS);

	HAL_PWREx_EnableUSBVoltageDetector();
}
