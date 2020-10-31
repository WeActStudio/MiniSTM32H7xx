#include "hid_bootloader.h"
#include "usb_device.h"
#include "usbd_customhid.h"
#include "rtc.h"
#include <string.h>
#include "w25qxx_qspi.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

typedef void (*pFunction)(void);
uint8_t USB_RX_Buffer[CUSTOM_HID_EPOUT_SIZE];
uint8_t USB_TX_Buffer[CUSTOM_HID_EPIN_SIZE]; //USB data -> PC
uint8_t new_data_is_received = 0;
uint16_t erase_page = 0;

static uint8_t CMD_SIGNATURE[6] = {'W','e','A','c','t',':'}; // "WeAct: "
/* Command: <Send next data pack> */
// static uint8_t CMD_DATA_RECEIVED[7] = {'W','e','A','c','t',':',2};// "WeAct: <cmd>"

#define FW_Version "V1.2"

#define Flash_Size (0x1FFF7A22UL)

#define CMD_ResetPage (0x00)
#define CMD_Reboot 		(0x01)
#define CMD_FW_Ver 		(0x02)
#define CMD_Ack    		(0x03)
#define CMD_Erase  		(0x04)

static uint8_t pageData[SECTOR_SIZE];

static void write_flash_sector(uint32_t currentPage);
static uint32_t erase_app_flash(void);

void hid_bootloader_Run(void)
{
	static volatile uint32_t current_Page = (USER_CODE_OFFSET / 1024);
  static volatile uint16_t currentPageOffset = 0;
	
	if (new_data_is_received == 1) {
		new_data_is_received = 0;
		if (memcmp(USB_RX_Buffer, CMD_SIGNATURE, sizeof (CMD_SIGNATURE)) == 0) 
		{
			switch(USB_RX_Buffer[6])
			{
				case CMD_ResetPage:

				/*------------ Reset pages */
				current_Page = 0;
				currentPageOffset = 0;
				erase_page = 0;
				break;

				case CMD_Reboot:
				HAL_Delay(10);
				USBD_DeInit(&hUsbDeviceFS);
				HAL_Delay(10);
				HAL_NVIC_SystemReset();
				break;
				
				case CMD_FW_Ver:
					memcpy(USB_TX_Buffer,CMD_SIGNATURE,sizeof(CMD_SIGNATURE));
        sprintf((char *)&USB_TX_Buffer[sizeof(CMD_SIGNATURE)],"%s 0x%X QSPIFlash: 0x%X APP Start: 0x90000000",FW_Version,HAL_GetDEVID(),w25qxx_ID);
					USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, USB_TX_Buffer, sizeof(USB_TX_Buffer));
				break;
				case CMD_Erase:
					memcpy(USB_TX_Buffer,CMD_SIGNATURE,sizeof(CMD_SIGNATURE));
					sprintf((char *)&USB_TX_Buffer[sizeof(CMD_SIGNATURE)],"%dKB",erase_app_flash());
					USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, USB_TX_Buffer, sizeof(USB_TX_Buffer));
				break;
			}
		}
		else 
		{
			memcpy(pageData + currentPageOffset, USB_RX_Buffer, HID_RX_SIZE);
			currentPageOffset += HID_RX_SIZE;
			if (currentPageOffset == SECTOR_SIZE) 
			{
				write_flash_sector(current_Page);
				current_Page++;
				currentPageOffset = 0;
				
				memset(USB_TX_Buffer, 0, sizeof(USB_TX_Buffer)); 
				USB_TX_Buffer[sprintf((char *)&USB_TX_Buffer,"WeAct:")] = CMD_Ack;
				//ST usb库存在异常
				USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, USB_TX_Buffer, sizeof(USB_TX_Buffer)); // 使用AC6编译正常 AC5进硬件错误
			}
		}
	}
}

static void write_flash_sector(uint32_t currentPage) 
{
  uint32_t pageAddress = 0 + (currentPage * SECTOR_SIZE);

  HAL_GPIO_WritePin(PE3_GPIO_Port, PE3_Pin, GPIO_PIN_RESET);	
  
  if(currentPage % 4 == 0)
  {
    W25qxx_EraseSector(erase_page*4096);
    erase_page ++;
  }
  
  W25qxx_WriteNoCheck(pageData,pageAddress,SECTOR_SIZE);
  
  HAL_GPIO_WritePin(PE3_GPIO_Port, PE3_Pin, GPIO_PIN_SET);  
}

// return	擦除扇区大小 单位 KB
static uint32_t erase_app_flash(void) 
{
	uint32_t data;
	uint16_t erase_app_page = 0;
	uint16_t erase_app_sector = 0;
	uint16_t flash_size;
	
	
  if(w25qxx_ID == 0xEF16)
    flash_size = 8192;
	else if(w25qxx_ID == 0xEF17)
    flash_size = 16384;
  
  W25qxx_Read((uint8_t *)&data,0,sizeof(data));
  
	/* Starts from 0x0 */
	for(erase_app_page=0;erase_app_page<flash_size;)
	{
		if((data != 0xFFFFFFFF))
		{
			HAL_GPIO_WritePin(PE3_GPIO_Port, PE3_Pin, GPIO_PIN_SET);	
      
			W25qxx_EraseSector(erase_app_sector*4096);
			
			if(erase_app_page % 4 == 0)
				erase_app_page += 4;
			
      W25qxx_Read((uint8_t *)&data,erase_app_page * 1024,sizeof(data));
			erase_app_sector++;
      
		}
		else
		{
			break;
		}
	}

	return (erase_app_page);
}
