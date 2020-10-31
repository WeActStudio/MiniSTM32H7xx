
#include "usbd_uvc.h"

uint8_t  USBD_UVC_Init (USBD_HandleTypeDef *pdev,
                            uint8_t cfgidx);

uint8_t  USBD_UVC_DeInit (USBD_HandleTypeDef *pdev,
                              uint8_t cfgidx);

uint8_t  USBD_UVC_Setup (USBD_HandleTypeDef *pdev,
                             USBD_SetupReqTypedef *req);

uint8_t  USBD_UVC_EP0_RxReady (USBD_HandleTypeDef *pdev);

uint8_t  USBD_UVC_DataIn (USBD_HandleTypeDef *pdev,
                              uint8_t epnum);


uint8_t  USBD_UVC_DataOut (USBD_HandleTypeDef *pdev,
                               uint8_t epnum);

uint8_t  USBD_UVC_SOF (USBD_HandleTypeDef *pdev);

uint8_t  *USBD_UVC_GetFSCfgDesc (uint16_t *length);

uint8_t  *USBD_UVC_GetUsrStrDescriptor(USBD_HandleTypeDef *pdev ,uint8_t index,  uint16_t *length);

void UVC_Req_GetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);

void UVC_Req_SetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);

__ALIGN_BEGIN uint8_t USBD_UVC_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

USBD_ClassTypeDef  USBD_UVC =
{
  USBD_UVC_Init,
  USBD_UVC_DeInit,
  USBD_UVC_Setup,
  NULL, /*EP0_TxSent*/
  USBD_UVC_EP0_RxReady, /*EP0_RxReady*/
  USBD_UVC_DataIn,
  USBD_UVC_DataOut,
  USBD_UVC_SOF, /*SOF */
  NULL,
  NULL,
  NULL,
  USBD_UVC_GetFSCfgDesc,
  NULL,
  NULL,
  USBD_UVC_GetUsrStrDescriptor
};

/* USB Vide Class device Configuration Descriptor */
uint8_t USBD_UVC_CfgFSDesc[USB_UVC_CONFIG_DESC_TOTAL_SIZE]  __ALIGN_END =
{
  /* Configuration 1 */
  9,   /* bLength: Configuation Descriptor size */
  USB_DESC_TYPE_CONFIGURATION,   /* bDescriptorType: Configuration */
  WBVAL(USB_UVC_CONFIG_DESC_TOTAL_SIZE), // wTotalLength
  2,   /* bNumInterfaces: 2 interfaces */
  0x01,   /* bConfigurationValue: Configuration's Id */
  UVC_USR_STR_CONFIG,   /* iConfiguration: */
  USB_CONFIG_BUS_POWERED,   /* bmAttributes: */
  USB_CONFIG_POWER_MA(100),   /* bMaxPower: 100 mA */

  /* Interface Association Descriptor */
  8, /* bLength */
  USB_DESC_TYPE_INTERFACE_ASSOCIATION, /* bDescriptorType */
  0x00, /* bFirstInterface */
  2, /* bInterfaceCount */
  CC_VIDEO,                      /* bFunctionClass     */
  SC_VIDEO_INTERFACE_COLLECTION, /* bFunctionSubClass  */
  PC_PROTOCOL_UNDEFINED,         /* bInterfaceProtocol */
  UVC_USR_STR_IAD, /* iFunction */

  /* VideoControl Interface Descriptor */

  /* Standard VC Interface Descriptor = interface 0 */
  9, /* bLength */
  USB_DESC_TYPE_INTERFACE, /* bDescriptorType */
  0x00, /* bInterfaceNumber */
  0x00, /* bAlternateSetting */
  0x00, /* bNumEndpoints */
  CC_VIDEO, /* bInterfaceClass */
  SC_VIDEOCONTROL, /* bInterfaceSubClass */
  PC_PROTOCOL_UNDEFINED, /* bInterfaceProtocol */
  UVC_USR_STR_VC_ITF, /* iInterface */

  /* Class-specific VC Interface Descriptor */
  12+1, /* bLength */
  CS_INTERFACE, /* bDescriptorType */
  VC_HEADER, /* bDescriptorSubType */
  WBVAL(0x0100), /* bcdVDC: UVC 1.0 */
  WBVAL(VC_DESC_TOTAL_SIZE), /* wTotalLength */
  DBVAL(6000000), /* dwClockFrequency: 6.000000 MHz - why 6MHz? - don't know */
  1, /* bInCollection */
  1, /* baInterfaceNr(1) */

  /* Camera Terminal Descriptor */
  15+2, /* bLength */
  CS_INTERFACE, /* bDescriptorType */
  VC_INPUT_TERMINAL, /* bDescriptorSubtype */
  0x01, /* bTerminalID */
  WBVAL(ITT_CAMERA), /* wTerminalType */
  0x00, /* bAssocTerminal */
  UVC_USR_STR_VC_IT, /* iTerminal */
  WBVAL(0x0000), /* wObjectiveFocalLengthMin */
  WBVAL(0x0000), /* wObjectiveFocalLengthMax */
  WBVAL(0x0000), /* wOcularFocalLength       */
  2,             /* bControlSize */
  0x00, 0x00,    /* bmControls */

  /* Output Terminal Descriptor */
  9, /* bLength */
  CS_INTERFACE, /* bDescriptorType */
  VC_OUTPUT_TERMINAL, /* bDescriptorSubtype */
  0x02, /* bTerminalID */
  WBVAL(TT_STREAMING), /* wTerminalType */
  0x00, /* bAssocTerminal */
  0x01, /* bSourceID: is connected to terminal 0x01 */
  UVC_USR_STR_VC_OT, /* iTerminal */

  /* VideoStreaming Interface Descriptor */

  /* Standard VS Interface Descriptor  = interface 1 */
  // alternate setting 0 = Zero Bandwidth
  9, /* bLength */
  USB_DESC_TYPE_INTERFACE, /* bDescriptorType */
  0x01, /* bInterafaceNumber */
  0x00, /* bAlternateSetting */
  0, /* bNumEndpoints */
  CC_VIDEO, /* bInterafaceClass */
  SC_VIDEOSTREAMING, /* bInterafaceSubClass */
  PC_PROTOCOL_UNDEFINED, /* bInterafaceProtocol */
  UVC_USR_STR_VS_ITF_AS0, /* iInterface */

  /* Class-specific VS Interface Input Header Descriptor */
  13+1*1, /* bLength */
  CS_INTERFACE,                              /* bDescriptorType */
  VS_INPUT_HEADER,                           /* bDescriptorSubtype */
  1,                                         /* bNumFormats */
  WBVAL(VS_DESC_TOTAL_SIZE), /* wTotalLength */
  USB_ENDPOINT_IN(1),                        /* bEndPointAddress */
  0x00,                                      /* bmInfo */
  0x02,                                      /* bTerminalLink: it's endpoint is connected to terminal 0x02 */
  0x00,                                      /* bStillCaptureMethod */
  0x01,                                      /* bTriggerSupport */
  0x00,                                      /* bTriggerUsage */
  1,                                         /* bControlSize */
  0x00,                                      /* bmaControls(1) */

  /* Uncompressed Video Format Descriptor */
  27, /* bLength */
  CS_INTERFACE,                         /* bDescriptorType */
  VS_FORMAT_UNCOMPRESSED,               /* bDescriptorSubType */
  0x01,                                 /* bFormatIndex */
  1,                                    /* bNumFrameDescriptors */
  0x59,0x55,0x59,0x32,                  /* Giud Format YUY2 {32595559-0000-0010-8000-00AA00389B71} */
  0x00,0x00,
  0x10,0x00,
  0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71,
  16,                                   /* bBitsPerPixel: 16 for yuy2 */
  0x01,                                 /* bDefaultFrameIndex: Default frame index is 1 */
  0x00,                                 /* bAspectRatioX */
  0x00,                                 /* bAspectRatioY */
  0x00,                                 /* bmInterlaceFlags */
  0x00,                                 /* bCopyProtect */

  /* Uncompressed Video Frame Descriptor */
  30, /* bLength */
  CS_INTERFACE, /* bDescriptorType */
  VS_FRAME_UNCOMPRESSED, /* bDscriptorSubtype */
  0x01, /* bFrameIndex */
  0x02, /* bmCapabilities: fixed frame-rate */
  WBVAL(UVC_VIDEO_WIDTH), /* wWidth */
  WBVAL(UVC_VIDEO_HEIGHT), /* wHeight */
  DBVAL(UVC_VIDEO_BITRATE), /* dwMinBitRate */
  DBVAL(UVC_VIDEO_BITRATE), /* dwMaxBitRate */
  DBVAL(UVC_VIDEO_MAX_FRAME_BUF_SIZE), /* dwMaxVideoFrameBufSize */
  DBVAL(UVC_VIDEO_INTERVAL), /* dwDefaultFrameInterval: for 5 FPS */
  1, /* bFrameIntervalType */
  DBVAL(UVC_VIDEO_INTERVAL), /* dwFrameInterval(1) */

  /* Color Matching Descriptor */
  6, /* bLength */
  CS_INTERFACE, /* bDescriptorType */
  VS_COLORFORMAT, /* bDescriptorSubtype */
  1, /* bColorPrimaries: 1 - BT.709, sRGB */
  1, /* bTransferCharacteristics: 1 - BT.709 */
  4, /* bMatrixCoefficients: 4 - SMPTE 170M */

  /* Standard VS Interface Descriptor  = interface 1 */
  // alternate setting 1 = operational setting
  9, /* bLength */
  USB_DESC_TYPE_INTERFACE, /* bDescriptorType */
  0x01, /* bInterafaceNumber */
  0x01, /* bAlternateSetting */
  1, /* bNumEndpoints */
  CC_VIDEO, /* bInterafaceClass */
  SC_VIDEOSTREAMING, /* bInterafaceSubClass */
  PC_PROTOCOL_UNDEFINED, /* bInterafaceProtocol */
  UVC_USR_STR_VS_ITF_AS1, /* iInterface */

  /* Standard VS Isochronous Video Data Endpoint Descriptor */
  7, /* bLength */
  USB_DESC_TYPE_ENDPOINT, /* bDescriptorType */
  USB_ENDPOINT_IN(1), /* bEndpointAddress */
  0x01, /* bmAttributes: 1 - Isochronous, 2 - Bulk */
  WBVAL(UVC_IN_EP1_PACKET_SIZE), /* wMaxPacketSize */
  1 /* bInterval */
};

//data array for Video Probe and Commit
VideoControl    videoCommitControl =
{
  {0x00,0x00},                      // bmHint
  {0x01},                           // bFormatIndex
  {0x01},                           // bFrameIndex
  {DBVAL(UVC_VIDEO_INTERVAL),},                // dwFrameInterval
  {0x00,0x00,},                     // wKeyFrameRate
  {0x00,0x00,},                     // wPFrameRate
  {0x00,0x00,},                     // wCompQuality
  {0x00,0x00,},                     // wCompWindowSize
  {0x00,0x00},                      // wDelay
  {DBVAL(UVC_VIDEO_MAX_FRAME_BUF_SIZE)},               // dwMaxVideoFrameSize
  {0x00, 0x00, 0x00, 0x00},         // dwMaxPayloadTransferSize
  {0x00, 0x00, 0x00, 0x00},         // dwClockFrequency
  {0x00},                           // bmFramingInfo
  {0x00},                           // bPreferedVersion
  {0x00},                           // bMinVersion
  {0x00},                           // bMaxVersion
};

VideoControl    videoProbeControl =
{
  {0x00,0x00},                      // bmHint
  {0x01},                           // bFormatIndex
  {0x01},                           // bFrameIndex
  {DBVAL(UVC_VIDEO_INTERVAL),},     // dwFrameInterval
  {0x00,0x00,},                     // wKeyFrameRate
  {0x00,0x00,},                     // wPFrameRate
  {0x00,0x00,},                     // wCompQuality
  {0x00,0x00,},                     // wCompWindowSize
  {0x00,0x00},                      // wDelay
  {DBVAL(UVC_VIDEO_MAX_FRAME_BUF_SIZE)},               // dwMaxVideoFrameSize
  {0x00, 0x00, 0x00, 0x00},         // dwMaxPayloadTransferSize
  {0x00, 0x00, 0x00, 0x00},         // dwClockFrequency
  {0x00},                           // bmFramingInfo
  {0x00},                           // bPreferedVersion
  {0x00},                           // bMinVersion
  {0x00},                           // bMaxVersion
};

uint8_t  USBD_UVC_RegisterCamera (USBD_HandleTypeDef *pdev,
                                  USBD_UVC_CameraTypeDef *camera)
{
  if (camera != NULL)
  {
    pdev->pUserData = camera;
  }
  return 0;
}

uint8_t  USBD_UVC_Init (USBD_HandleTypeDef *pdev,
                        uint8_t cfgidx)
{

  /* Open EP IN */
  USBD_LL_OpenEP(pdev,
  		USB_ENDPOINT_IN(1),
        USBD_EP_TYPE_ISOC,
		UVC_IN_EP1_PACKET_SIZE);

  //uint32_t debug_otg_fs_diepctl1 = *((uint32_t*)(USB_OTG_FS_PERIPH_BASE + USB_OTG_IN_ENDPOINT_BASE + (1 * USB_OTG_EP_REG_SIZE)));

  pdev->pClassData = USBD_malloc(sizeof (USBD_UVC_HandleTypeDef));

  if(pdev->pClassData == NULL)
  {
	  return USBD_FAIL;
  }

  USBD_UVC_HandleTypeDef *huvc = (USBD_UVC_HandleTypeDef*) pdev->pClassData;
  huvc->interface = 0;
  huvc->state = UVC_STATE_OFF;
  huvc->frame_count = 0;
  huvc->current_frame_number = 0;
  huvc->current_frame = NULL;
  huvc->current_frame_length = 0;
  huvc->current_frame_sent = 0;

  return USBD_OK;
}

uint8_t  USBD_UVC_DeInit (USBD_HandleTypeDef *pdev,
                          uint8_t cfgidx)
{
  /* Close EP IN */
  USBD_LL_CloseEP(pdev, USB_ENDPOINT_IN(1));

  /* Free MSC Class Resources */
  if(pdev->pClassData != NULL)
  {
    USBD_free(pdev->pClassData);
    pdev->pClassData = NULL;
  }

  return USBD_OK;
}
USBD_UVC_HandleTypeDef *huvc;
uint8_t  USBD_UVC_Setup (USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
	uint8_t ret = USBD_OK;
  huvc = (USBD_UVC_HandleTypeDef*) pdev->pClassData;
  USBD_UVC_CameraTypeDef *camera = (USBD_UVC_CameraTypeDef*) pdev->pUserData;

  switch (req->bmRequest & USB_REQ_TYPE_MASK)
  {

		/* Class request */
		case USB_REQ_TYPE_CLASS :
			switch (req->bRequest)
			{
				case UVC_GET_CUR:
				case UVC_GET_DEF:
				case UVC_GET_MIN:
				case UVC_GET_MAX:
					UVC_Req_GetCurrent(pdev, req);
					break;

				case UVC_SET_CUR:
					UVC_Req_SetCurrent(pdev, req);
					break;

				default:
					 USBD_CtlError(pdev, req);
					 ret = USBD_FAIL;
			}
			break;
		/* Interface & Endpoint request */
		case USB_REQ_TYPE_STANDARD:
			switch (req->bRequest)
			{

				case USB_REQ_GET_DESCRIPTOR:
					break;

				case USB_REQ_GET_INTERFACE:
					USBD_CtlSendData (pdev,
														(uint8_t *)&huvc->interface,
														1);
					break;

				case USB_REQ_SET_INTERFACE:
					huvc->interface = (uint8_t)(req->wValue);
					if (huvc->interface == 1) {
					huvc->state = UVC_STATE_READY;
					huvc->current_frame_number = 0;
					if (camera != NULL && camera->UvcOn != NULL)
					{
						camera->UvcOn();
					}
					} else {
						//USBD_LL_FlushEP(pdev, USB_ENDPOINT_IN(1));
						huvc->state = UVC_STATE_OFF;
						if (camera != NULL && camera->UvcOff != NULL)
						{
								camera->UvcOff();
						}
					}
					break;
			}
			break;

		default:
			USBD_CtlError(pdev, req);
      ret = USBD_FAIL;
			break;
	}
	return ret;
}

uint8_t  USBD_UVC_EP0_RxReady (USBD_HandleTypeDef *pdev)
{
  return USBD_OK;
}

uint8_t  USBD_UVC_DataIn (USBD_HandleTypeDef *pdev,
                          uint8_t epnum)
{
  USBD_UVC_HandleTypeDef *huvc = (USBD_UVC_HandleTypeDef*) pdev->pClassData;
  USBD_UVC_CameraTypeDef *camera = (USBD_UVC_CameraTypeDef*) pdev->pUserData;
 
  if (huvc->state == UVC_STATE_NEED_FRAME)
  {
	  if (camera != NULL)
	  {
		  uint32_t frame_length;
		  uint8_t *frame = camera->GetFrame(&frame_length);
		  if (frame != NULL)
		  {
			  huvc->current_frame = frame;
			  huvc->current_frame_length = frame_length;
			  huvc->current_frame_sent = 0;
			  huvc->state = UVC_STATE_BUSY;
		  }
	  }
	  else
	  {
		  huvc->current_frame = NULL;
		  huvc->current_frame_length = UVC_VIDEO_WIDTH * UVC_VIDEO_HEIGHT * 2;
		  huvc->current_frame_sent = 0;
		  huvc->state = UVC_STATE_BUSY;
	  }
  }

  uint32_t packet_size = 0;
  uint8_t packet[UVC_IN_EP1_PACKET_SIZE];
  uint8_t header[2] = {2, huvc->current_frame_number % 2};

  packet[0] = header[0];
  packet[1] = header[1];

  packet_size = 2;

  if (huvc->state == UVC_STATE_BUSY)
  {
	  while (packet_size < UVC_IN_EP1_PACKET_SIZE && huvc->current_frame_sent < huvc->current_frame_length)
	  {
		  if (huvc->current_frame == NULL)
		  {
			  if (huvc->current_frame_sent % 2 == 0) {
				  packet[packet_size] = (((huvc->frame_count % 10) > 5) ? (10-(huvc->frame_count % 10)) : (huvc->frame_count % 10)) * 200 / 5;
			  } else {
				  packet[packet_size] = 128;
			  }
		  }
		  else
		  {
			  packet[packet_size] = huvc->current_frame[huvc->current_frame_sent];
		  }
		  packet_size++;
		  huvc->current_frame_sent++;
	  }
  }

  if (huvc->state == UVC_STATE_NEED_FRAME || huvc->state == UVC_STATE_BUSY)
  {
	  USBD_LL_Transmit(pdev, USB_ENDPOINT_IN(1), (uint8_t*)&packet, (uint32_t)packet_size);
  }

  if (huvc->state == UVC_STATE_BUSY)
  {
	  if (huvc->current_frame_sent >= huvc->current_frame_length)
	  {
		  huvc->frame_count++;

		  if (camera != NULL && camera->FreeFrame != NULL && huvc->current_frame != NULL)
		  {
		      camera->FreeFrame(huvc->current_frame);
		      huvc->current_frame = NULL;
		  }

		  huvc->state = UVC_STATE_NEED_FRAME;
		  huvc->current_frame_number++;
	  }
  }

  return USBD_OK;
}

uint8_t  USBD_UVC_DataOut (USBD_HandleTypeDef *pdev,
                           uint8_t epnum)
{
  return USBD_OK;
}

uint8_t  *USBD_UVC_GetFSCfgDesc (uint16_t *length)
{
  *length = USB_UVC_CONFIG_DESC_TOTAL_SIZE;
  return USBD_UVC_CfgFSDesc;
}

uint8_t  USBD_UVC_SOF (USBD_HandleTypeDef *pdev)
{
  USBD_UVC_HandleTypeDef *huvc = (USBD_UVC_HandleTypeDef*) pdev->pClassData;
  
  if (huvc->state == UVC_STATE_READY)
  {
		uint8_t payload[2] = {0x02,0x00};
	  USBD_LL_Transmit (pdev, USB_ENDPOINT_IN(1), payload, 2);
    huvc->state = UVC_STATE_NEED_FRAME;
  }
  return USBD_OK;
}

uint8_t  *USBD_UVC_GetUsrStrDescriptor(USBD_HandleTypeDef *pdev ,uint8_t index,  uint16_t *length)
{
  switch (index)
  {
  case UVC_USR_STR_IAD:
	  USBD_GetString ((uint8_t *)&UVC_NAME, USBD_UVC_StrDesc, length);
	  return USBD_UVC_StrDesc;
  default:
	  break;
  }
  USBD_GetString ((uint8_t *)&"USR_STR", USBD_UVC_StrDesc, length);
  return USBD_UVC_StrDesc;
}

void UVC_Req_GetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  /* Send the current mute state */

  if(req->wValue == 256)
  {
    //Probe Request
    USBD_CtlSendData (pdev, (uint8_t*)&videoProbeControl, req->wLength);
  }
  else if (req->wValue == 512)
  {
    //Commit Request
    USBD_CtlSendData (pdev, (uint8_t*)&videoCommitControl, req->wLength);
  }
}

void UVC_Req_SetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
  if (req->wLength)
  {
    /* Prepare the reception of the buffer over EP0 */
    if(req->wValue == 256)
    {
      //Probe Request
      USBD_CtlPrepareRx (pdev, (uint8_t*)&videoProbeControl, req->wLength);
    }
    else if (req->wValue == 512)
    {
      //Commit Request
      USBD_CtlPrepareRx (pdev, (uint8_t*)&videoCommitControl, req->wLength);
    }

  }
}
