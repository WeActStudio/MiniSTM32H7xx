# WeAct Studio
## ExtMem Boot USB HID

在8MB QSPI Flash上运行程序，该例程为QSPI Bootloader,APP Addr: `0x90000000`

1. 按住K1键上电或复位，LED慢闪时松开K1键进入WeAct HID Flash，可以使用WeAct HID Flash 上位机烧录QSPIFlash程序，QSPI时钟源为D1HCLK，120Mhz
2. 按住K1键上电或复位，LED慢闪，继续按住K1键，LED变为快闪，松开K1键进入DFU模式

Run the program on 8MB QSPI Flash, this routine is QSPI Bootloader,APP Addr: `0x90000000`

1. Press the K1 key to power up or reset, and release the K1 key to enter WeAct HID Flash when LED is slow flashing. You can use the WeAct HID Flash host computer to burn QSPIFlash program with the QSPI clock source of D1HCLK, 120Mhz
2. Press and hold K1 to power up or reset, and the LED will flash slowly. Continue to press and hold K1, and the LED will turn into flash, and release K1 to enter DFU mode
