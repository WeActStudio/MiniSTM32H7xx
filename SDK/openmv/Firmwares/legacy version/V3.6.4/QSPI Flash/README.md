# 中文说明
## QSPI Flash Firmware
Openmv 固件放在外部8MB QSPI Flash，所以需要下载0x08000000.hex，初始化QSPI外设，将QSPI Flash映射到0x90000000。
### 固件下载地址定义
0x08000000.hex -> 0x08000000

0x90000000.bin -> 0x90000000

> 0x90000000.bin 固件需要使用STM32CubeProg软件，勾选使能QSPI Flash下载算法，使用ST-Link进行下载

> Bin文件需要自己定义下载地址，请注意，STM32CubeProg `Download`按钮右边有向下的`小箭头`，点击可以自定义下载地址

# English Description
## QSPI Flash
The Openmv firmware is placed on the external 8MB QSPI Flash, so download 0x080000000.hex, initialize the QSPI peripheral, and map QSPI Flash to 0x90000000.
### firmware download address definition
0x08000000.hex -> 0x08000000

0x90000000.bin -> 0x90000000

> 0x90000000.bin firmware needs to use STM32CubeProg software, check enable QSPI Flash download algorithm, use ST-Link to download

> Bin file need to define the Download address, please note that STM32CubeProg `Download` button on the right side of the down `arrow`, click to customize the Download address

