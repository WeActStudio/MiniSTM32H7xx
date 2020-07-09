# 中文说明
## Internal Flash
> Openmv 固件放在MCU内部存储地址，从0x08000000开始

固件下载需要使用USB模式下载，ST-Link下载会有大小限制，可以使用`WeAct Studio USB Download Tool`（位于Soft文件夹）或者使用STM32CubeProg进行下载，建议使用`WeAct Studio USB Download Tool`烧录openmv.bin文件，百度网盘有下载烧录视频教程

## QSPI Flash
> Openmv 固件放在外部8MB QSPI Flash，所以需要下载0x08000000.hex，初始化QSPI外设，将QSPI Flash映射到0x90000000。

固件下载需要使用STM32CubeProg软件，勾选使能QSPI Flash下载算法，使用ST-Link进行下载

> Bin文件需要自己定义下载地址，请注意，STM32CubeProg `Download`按钮右边有向下的`小箭头`，点击可以自定义下载地址

# English Description
## Internal Flash
> The Openmv firmware download is at the internal address, starting at 0x08000000

> Firmware Download needs to use USB mode, st-link Download will have size limit, you can use  `WeAct Studio USB Download Tool` (located in `Soft` folder) or `STM32CubeProg` to Download, it is recommended to use `WeAct Studio USB Download Tool` to burn openmv.bin file

## QSPI Flash
> The Openmv firmware is placed on the external 8MB QSPI Flash, so download 0x080000000.hex, initialize the QSPI peripheral, and map QSPI Flash to 0x90000000.

Firmware needs to use STM32CubeProg software, check enable QSPI Flash download algorithm, use ST-Link to download

> Bin file need to define the Download address, please note that STM32CubeProg `Download` button on the right side of the down `arrow`, click to customize the Download address

