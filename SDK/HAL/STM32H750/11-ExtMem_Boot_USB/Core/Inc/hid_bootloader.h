#ifndef __hid_bootloader_H
#define __hid_bootloader_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
#include "usbd_core.h"

// <USER CODE> flash start address.
#define USER_CODE_OFFSET  0x0000UL
#define SECTOR_SIZE   1024
#define HID_RX_SIZE   64

void hid_bootloader_Run(void);
#ifdef __cplusplus
}
#endif

#endif

