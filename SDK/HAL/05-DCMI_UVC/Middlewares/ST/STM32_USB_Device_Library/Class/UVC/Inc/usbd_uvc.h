#ifndef __USBD_UVC_H
#define __USBD_UVC_H


#ifdef __cplusplus
 extern "C" {
#endif

#include  "usbd_ioreq.h"

 //UVC 1.0 uses only 26 first bytes
 typedef struct _VideoControl{
   uint8_t    bmHint[2];                      // 2
   uint8_t    bFormatIndex[1];                // 3
   uint8_t    bFrameIndex[1];                 // 4
   uint8_t    dwFrameInterval[4];             // 8
   uint8_t    wKeyFrameRate[2];               // 10
   uint8_t    wPFrameRate[2];                 // 12
   uint8_t    wCompQuality[2];                // 14
   uint8_t    wCompWindowSize[2];             // 16
   uint8_t    wDelay[2];                      // 18
   uint8_t    dwMaxVideoFrameSize[4];         // 22
   uint8_t    dwMaxPayloadTransferSize[4];    // 26
   uint8_t    dwClockFrequency[4];
   uint8_t    bmFramingInfo[1];
   uint8_t    bPreferedVersion[1];
   uint8_t    bMinVersion[1];
   uint8_t    bMaxVersion[1];
 } VideoControl;

#define UVC_NAME "WeAct Studio Camera"

//#define UVC_MAX_FS_PACKET            64
//#define UVC_MAX_HS_PACKET            0x200

// in bytes
#define UVC_IN_EP1_PACKET_SIZE         (512)

#define UVC_VIDEO_WIDTH        160
#define UVC_VIDEO_HEIGHT       120
#define UVC_VIDEO_MAX_FRAME_BUF_SIZE (UVC_VIDEO_WIDTH*UVC_VIDEO_HEIGHT*2)
#define UVC_VIDEO_FPS          14
#define UVC_VIDEO_BITRATE      (UVC_VIDEO_MAX_FRAME_BUF_SIZE*8*UVC_VIDEO_FPS)
// in 100ns
#define UVC_VIDEO_INTERVAL     (100000000/UVC_VIDEO_FPS)

#define VC_DESC_TOTAL_SIZE ((12+1)+(15+2)+9)
#define VS_DESC_TOTAL_SIZE ((13+1*1)+27+30+6)
#define USB_UVC_CONFIG_DESC_TOTAL_SIZE (9+8+9+VC_DESC_TOTAL_SIZE+9+VS_DESC_TOTAL_SIZE+9+7)

 // Video Interface Class Codes
 // (USB_Video_Class_1.1.pdf, A.1 Video Interface Class Code)
 #define CC_VIDEO                                   0x0E

 // Video Interface Subclass Codes
 // (USB_Video_Class_1.1.pdf, A.2 Video Interface Subclass Code)
 #define SC_UNDEFINED                               0x00
 #define SC_VIDEOCONTROL                            0x01
 #define SC_VIDEOSTREAMING                          0x02
 #define SC_VIDEO_INTERFACE_COLLECTION              0x03

 // Video Interface Protocol Codes
 // (USB_Video_Class_1.1.pdf, A.3 Video Interface Protocol Codes)
 #define PC_PROTOCOL_UNDEFINED                      0x00

 // Video Class-Specific Descriptor Types
 // (USB_Video_Class_1.1.pdf, A.4 Video Class-Specific Descriptor Types)
 #define CS_UNDEFINED                               0x20
 #define CS_DEVICE                                  0x21
 #define CS_CONFIGURATION                           0x22
 #define CS_STRING                                  0x23
 #define CS_INTERFACE                               0x24
 #define CS_ENDPOINT                                0x25

 // Video Class-Specific VideoControl Interface Descriptor Subtypes
 // (USB_Video_Class_1.1.pdf, A.5 Video Class-Specific VC Interface Descriptor Subtypes)
 #define VC_DESCRIPTOR_UNDEFINED                    0x00
 #define VC_HEADER                                  0x01
 #define VC_INPUT_TERMINAL                          0x02
 #define VC_OUTPUT_TERMINAL                         0x03
 #define VC_SELECTOR_UNIT                           0x04
 #define VC_PROCESSING_UNIT                         0x05
 #define VC_EXTENSION_UNIT                          0x06

 // Video Class-Specific VideoStreaming Interface Descriptor Subtypes
 // (USB_Video_Class_1.1.pdf, A.6 Video Class-Specific VS Interface Descriptor Subtypes)
 #define VS_UNDEFINED                               0x00
 #define VS_INPUT_HEADER                            0x01
 #define VS_OUTPUT_HEADER                           0x02
 #define VS_STILL_IMAGE_FRAME                       0x03
 #define VS_FORMAT_UNCOMPRESSED                     0x04
 #define VS_FRAME_UNCOMPRESSED                      0x05
 #define VS_FORMAT_MJPEG                            0x06
 #define VS_FRAME_MJPEG                             0x07
 #define VS_FORMAT_MPEG2TS                          0x0A
 #define VS_FORMAT_DV                               0x0C
 #define VS_COLORFORMAT                             0x0D
 #define VS_FORMAT_FRAME_BASED                      0x10
 #define VS_FRAME_FRAME_BASED                       0x11
 #define VS_FORMAT_STREAM_BASED                     0x12



 // Video Class-Specific Request Codes
 // (USB_Video_Class_1.1.pdf, A.8 Video Class-Specific Request Codes)
 #define RC_UNDEFINED                               0x00
 #define UVC_SET_CUR                                0x01
 #define UVC_GET_CUR                                0x81
 #define UVC_GET_MIN                                0x82
 #define UVC_GET_MAX                                0x83
 #define UVC_GET_RES                                0x84
 #define UVC_GET_LEN                                0x85
 #define UVC_GET_INFO                               0x86
 #define UVC_GET_DEF                                0x87



 // USB Terminal Types
 // (USB_Video_Class_1.1.pdf, B.1 USB Terminal Types)
 #define TT_VENDOR_SPECIFIC         	             0x0100
 #define TT_STREAMING               	             0x0101

 // Input Terminal Types
 // (USB_Video_Class_1.1.pdf, B.2 Input Terminal Types)
 #define ITT_VENDOR_SPECIFIC                      0x0200
 #define ITT_CAMERA                               0x0201
 #define ITT_MEDIA_TRANSPORT_INPUT                0x0202

 // Output Terminal Types
 // (USB_Video_Class_1.1.pdf, B.3 Output Terminal Types)
 #define OTT_VENDOR_SPECIFIC                      0x0300
 #define OTT_DISPLAY                              0x0301
 #define OTT_MEDIA_TRANSPORT_OUTPUT               0x0302

 // External Terminal Types
 // (USB_Video_Class_1.1.pdf, B.4 External Terminal Types)
 #define EXTERNAL_VENDOR_SPECIFIC                 0x0400
 #define COMPOSITE_CONNECTOR                      0x0401
 #define SVIDEO_CONNECTOR                         0x0402
 #define COMPONENT_CONNECTOR                      0x0403


#define WBVAL(x) (x & 0xFF),((x >> 8) & 0xFF)
#define DBVAL(x) (x & 0xFF),((x >> 8) & 0xFF),((x >> 16) & 0xFF),((x >> 24) & 0xFF)
/* bMaxPower in Configuration Descriptor */
#define USB_CONFIG_POWER_MA(mA)                ((mA)/2)
#define USB_CONFIG_BUS_POWERED                 0x80
#define  USB_DESC_TYPE_INTERFACE_ASSOCIATION   11
/* bEndpointAddress in Endpoint Descriptor */
#define USB_ENDPOINT_DIRECTION_MASK            0x80
#define USB_ENDPOINT_OUT(addr)                 ((addr) | 0x00)
#define USB_ENDPOINT_IN(addr)                  ((addr) | 0x80)

#define UVC_STATE_OFF 0
#define UVC_STATE_READY 1
#define UVC_STATE_NEED_FRAME 2
#define UVC_STATE_BUSY 3

 typedef struct
 {
   uint32_t interface;
   uint8_t  state;
   uint32_t frame_count;
   uint32_t current_frame_number;
   uint8_t *current_frame;
   uint32_t current_frame_length;
   uint32_t current_frame_sent;
 }
 USBD_UVC_HandleTypeDef;

#define UVC_USR_STR_CONFIG 0x11
#define UVC_USR_STR_IAD 0x12
#define UVC_USR_STR_VC_ITF 0x13
#define UVC_USR_STR_VC_IT 0x14
#define UVC_USR_STR_VC_OT 0x15
#define UVC_USR_STR_VS_ITF_AS0 0x16
#define UVC_USR_STR_VS_ITF_AS1 0x17

typedef struct {
 	void (*UvcOn)();
 	void (*UvcOff)();
 	uint8_t *(*GetFrame)(uint32_t *pFrameLength);
 	void (*FreeFrame)(uint8_t *frame);
}
USBD_UVC_CameraTypeDef;

uint8_t  USBD_UVC_RegisterCamera(USBD_HandleTypeDef   *pdev, USBD_UVC_CameraTypeDef *camera);

extern USBD_ClassTypeDef  USBD_UVC;

#ifdef __cplusplus
}
#endif

#endif  /* __USBD_UVC_H */
