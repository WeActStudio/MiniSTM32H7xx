# 中文说明
## QSPI Flash Firmware
Openmv 固件放在外部8MB QSPI Flash，所以需要下载0x08000000.hex，初始化QSPI外设，将QSPI Flash映射到0x90000000。
### 固件下载地址定义
0x08000000.hex -> 0x08000000

0x90000000.bin -> 0x90000000

> 0x90000000.bin 固件需要使用STM32CubeProg软件，勾选使能QSPI Flash下载算法，使用ST-Link进行下载

> Bin文件需要自己定义下载地址，请注意，STM32CubeProg `Download`按钮右边有向下的`小箭头`，点击可以自定义下载地址

### 0x08000000.hex 说明
1. 按住K1键上电或复位，LED慢闪时松开K1键进入WeAct HID Flash，可以使用WeAct HID Flash 上位机烧录openmv app: 0x90000000.bin
2. 按住K1键上电或复位，LED慢闪，继续按住K1键，LED变为快闪，松开K1键进入DFU模式
> 务必忽略WeAct HID Flash 上位机的工程设置步骤，那个步骤是针对STM32F4x1的芯片

# English Description
## QSPI Flash
The Openmv firmware is placed on the external 8MB QSPI Flash, so download 0x080000000.hex, initialize the QSPI peripheral, and map QSPI Flash to 0x90000000.
### firmware download address definition
0x08000000.hex -> 0x08000000

0x90000000.bin -> 0x90000000

> 0x90000000.bin firmware needs to use STM32CubeProg software, check enable QSPI Flash download algorithm, use ST-Link to download

> Bin file need to define the Download address, please note that STM32CubeProg `Download` button on the right side of the down `arrow`, click to customize the Download address

### 0x08000000.hex Description
1. Press the K1 key to power up or reset, and release the K1 key to enter WeAct HID Flash when LED is slow flashing. You can use the WeAct HID Flash host computer to burn openmv app: 0x90000000.bin
2. Press and hold K1 to power up or reset, and the LED will flash slowly. Continue to press and hold K1, and the LED will turn into flash, and release K1 to enter DFU mode
> Be sure to ignore the WeAct HID Flash host computer project Settings step, that step is for STM32F4x1 chip