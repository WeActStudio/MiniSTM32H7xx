/*
 * This file is part of the MicroPython project, http://micropython.org/
 * The MIT License (MIT)
 * Copyright (C) 2013-2025 OpenMV, LLC.
 */

#define MICROPY_HW_BOARD_NAME       "WeActStudio-H7x"
#define MICROPY_HW_MCU_NAME         "STM32H7xx"
#define MICROPY_PY_SYS_PLATFORM     "WeActStudio-H7x"
#define MICROPY_HW_FLASH_FS_LABEL   "WeActStudio"

// Network config
#define MICROPY_PY_NETWORK_HOSTNAME_DEFAULT "mpy-weact-h7x"

#define MICROPY_OBJ_REPR            (MICROPY_OBJ_REPR_C)
#define UINT_FMT                    "%u"
#define INT_FMT                     "%d"
typedef int mp_int_t;               // must be pointer size
typedef unsigned int mp_uint_t;     // must be pointer size

#define MICROPY_FATFS_EXFAT         (1)
#define MICROPY_HW_ENABLE_RTC       (1)
#define MICROPY_HW_ENABLE_RNG       (1)
#define MICROPY_HW_ENABLE_ADC       (1)
#define MICROPY_HW_ENABLE_DAC       (0)
#define MICROPY_HW_ENABLE_SPI2      (1)
#define MICROPY_HW_ENABLE_SPI4      (1)
#define MICROPY_HW_ENABLE_USB       (1)
#define MICROPY_HW_HAS_FLASH        (1)
#define MICROPY_HW_ENABLE_SERVO     (1)
#define MICROPY_HW_ENABLE_TIMER     (1)
#define MICROPY_HW_ENABLE_SDCARD    (1)
#define MICROPY_HW_ENTER_BOOTLOADER_VIA_RESET   (0)
#define MICROPY_HW_TIM_IS_RESERVED(id) (id == 1 || id == 6)

// ROMFS config
#define MICROPY_HW_ROMFS_ENABLE_INTERNAL_FLASH  (1)
#define MICROPY_HW_ROMFS_ENABLE_PART0           (1)

#define MICROPY_BOARD_ENTER_STANDBY

extern void board_enter_bootloader(void);
#define MICROPY_BOARD_ENTER_BOOTLOADER(nargs, args) board_enter_bootloader()

// Note these are not used in top system.c.
#define MICROPY_HW_CLK_PLLM         (5)
#define MICROPY_HW_CLK_PLLN         (160)
#define MICROPY_HW_CLK_PLLP         (2)
#define MICROPY_HW_CLK_PLLQ         (8)
#define MICROPY_HW_CLK_PLLR         (2)

// UART1 config
#define MICROPY_HW_UART1_TX                 (pin_B14)
#define MICROPY_HW_UART1_RX                 (pin_B15)
#define MICROPY_HW_UART2_TX                 (pin_A2)
#define MICROPY_HW_UART2_RX                 (pin_A3)
#define MICROPY_HW_UART3_TX                 (pin_B10)
#define MICROPY_HW_UART3_RX                 (pin_B11)
#define MICROPY_HW_UART3_RTS                (pin_B14)
#define MICROPY_HW_UART3_CTS                (pin_B13)
#define MICROPY_HW_UART4_TX                 (pin_C11)
#define MICROPY_HW_UART4_RX                 (pin_C10)
#define MICROPY_HW_UART5_TX                 (pin_B12) // or SPI2
#define MICROPY_HW_UART5_RX                 (pin_B13) // or SPI2
#define MICROPY_HW_UART7_TX                 (pin_E8)
#define MICROPY_HW_UART7_RX                 (pin_E7)

// I2C buses
#define MICROPY_HW_I2C2_SCL (pin_B10)
#define MICROPY_HW_I2C2_SDA (pin_B11)

#define MICROPY_HW_I2C4_SCL (pin_D12)
#define MICROPY_HW_I2C4_SDA (pin_D13)

// SPI buses
#define MICROPY_HW_SPI1_NSS  (pin_D6)
#define MICROPY_HW_SPI1_SCK  (pin_B3)
#define MICROPY_HW_SPI1_MISO (pin_B4)
#define MICROPY_HW_SPI1_MOSI (pin_D7)

#define MICROPY_HW_SPI2_NSS  (pin_B12)
#define MICROPY_HW_SPI2_SCK  (pin_B13)
#define MICROPY_HW_SPI2_MISO (pin_B14)
#define MICROPY_HW_SPI2_MOSI (pin_B15)

#define MICROPY_HW_SPI4_NSS  (pin_E11)
#define MICROPY_HW_SPI4_SCK  (pin_E12)
#define MICROPY_HW_SPI4_MISO (pin_E13)
#define MICROPY_HW_SPI4_MOSI (pin_E14)

// FDCAN bus
#define MICROPY_HW_CAN1_NAME  "FDCAN1"
#define MICROPY_HW_CAN1_TX    (pin_D1)
#define MICROPY_HW_CAN1_RX    (pin_D0)

#define MICROPY_HW_CAN2_NAME "FDCAN2"
#define MICROPY_HW_CAN2_TX   (pin_B13)
#define MICROPY_HW_CAN2_RX   (pin_B12)
#define MICROPY_HW_CAN_IS_RESERVED(id) (id != PYB_CAN_2)

// SD card detect switch
#define MICROPY_HW_SDCARD_DETECT_PIN        (pin_D4)
#define MICROPY_HW_SDCARD_DETECT_PULL       (GPIO_PULLUP)
#define MICROPY_HW_SDCARD_DETECT_PRESENT    (GPIO_PIN_SET)

// USB config
#define MICROPY_HW_USB_FS                   (1)
#define MICROPY_HW_USB_CDC_RX_DATA_SIZE     (512)
#define MICROPY_HW_USB_CDC_TX_DATA_SIZE     (512)

// LEDs
#define MICROPY_HW_LED1             (pin_E3) // blue

#define MICROPY_HW_LED_OTYPE        (GPIO_MODE_OUTPUT_PP)
// NOTE: LEDs are active high.
#define MICROPY_HW_LED_OFF(pin)      (pin->gpio->BSRR = (pin->pin_mask << 16))
#define MICROPY_HW_LED_ON(pin)     (pin->gpio->BSRR = pin->pin_mask)

// Servos
#define PYB_SERVO_NUM (4)

// Board network interfaces config.
#if MICROPY_PY_WINC1500
extern const struct _mp_obj_type_t mod_network_nic_type_winc;
#define MICROPY_PY_USOCKET_EXTENDED_STATE   (1)
#define MICROPY_BOARD_NETWORK_INTERFACES \
    { MP_ROM_QSTR(MP_QSTR_WINC), MP_ROM_PTR(&mod_network_nic_type_winc) },\
    { MP_ROM_QSTR(MP_QSTR_WLAN), MP_ROM_PTR(&mod_network_nic_type_winc) },
#else
#define MICROPY_BOARD_NETWORK_INTERFACES
#endif

#define MICROPY_HW_USB_VID                      0x37C5
#define MICROPY_HW_USB_PID                      0x1204
#define MICROPY_HW_USB_PID_CDC_MSC              (MICROPY_HW_USB_PID)
#define MICROPY_HW_USB_PID_CDC_HID              (MICROPY_HW_USB_PID)
#define MICROPY_HW_USB_PID_CDC                  (MICROPY_HW_USB_PID)
#define MICROPY_HW_USB_PID_MSC                  (MICROPY_HW_USB_PID)
#define MICROPY_HW_USB_PID_CDC2_MSC             (MICROPY_HW_USB_PID)
#define MICROPY_HW_USB_PID_CDC2                 (MICROPY_HW_USB_PID)
#define MICROPY_HW_USB_PID_CDC3                 (MICROPY_HW_USB_PID)
#define MICROPY_HW_USB_PID_CDC3_MSC             (MICROPY_HW_USB_PID)
#define MICROPY_HW_USB_PID_CDC_MSC_HID          (MICROPY_HW_USB_PID)
#define MICROPY_HW_USB_PID_CDC2_MSC_HID         (MICROPY_HW_USB_PID)
#define MICROPY_HW_USB_PID_CDC3_MSC_HID         (MICROPY_HW_USB_PID)
#define MICROPY_HW_USB_LANGID_STRING            0x409
#define MICROPY_HW_USB_MANUFACTURER_STRING      "OpenMV"
#define MICROPY_HW_USB_PRODUCT_FS_STRING        "OpenMV Virtual Comm Port in FS Mode"
#define MICROPY_HW_USB_PRODUCT_HS_STRING        "OpenMV Virtual Comm Port in HS Mode"
#define MICROPY_HW_USB_INTERFACE_FS_STRING      "VCP Interface"
#define MICROPY_HW_USB_INTERFACE_HS_STRING      "VCP Interface"
#define MICROPY_HW_USB_CONFIGURATION_FS_STRING  "VCP Config"
#define MICROPY_HW_USB_CONFIGURATION_HS_STRING  "VCP Config"
