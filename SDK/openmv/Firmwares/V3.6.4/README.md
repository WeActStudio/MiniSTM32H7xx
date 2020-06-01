# Internal Flash
固件下载在内部地址，从0x08000000开始
## 8MB QSPI Flash
8MB QSPI Flash 作为Python代码存储器
## 8MB SPI Flash
8MB SPI Flash 作为Python代码存储器
## 固件下载地址定义
bootloader.*  -> 0x08000000

firmware.*    -> 0x08040000

openmv.*      -> 0x08000000

openmv.* 为bootloader和firmware两个固件的合并固件，按需选择烧录即可

> 固件下载需要使用USB模式下载，ST-Link下载会有大小限制，可以使用`WeAct Studio USB Download Tool`（位于Soft文件夹）或者使用STM32CubeProg进行下载，建议使用`WeAct Studio USB Download Tool`烧录openmv.bin文件

# QSPI Flash
固件放置在外部8MB QSPI Flash，所以需要下载0x08000000.hex，初始化QSPI外设，将QSPI Flash映射到0x90000000。
## 固件下载地址定义
0x08000000.hex -> 0x08000000

0x90000000.bin -> 0x90000000

> 0x90000000.bin 固件需要使用STM32CubeProg软件，勾选使能QSPI Flash下载算法，使用ST-Link进行下载

> Bin文件需要自己定义下载地址，请注意，STM32CubeProg `Download`按钮右边有向下的`小箭头`，点击可以自定义下载地址


# Internal Flash
The firmware download is at the internal address, starting at 0x08000000
## 8 MB QSPI Flash
8MB QSPI Flash as Python code memory
## 8 MB SPI Flash
8MB SPI Flash as Python code memory
## firmware download address definition
bootloader.*  -> 0x08000000

firmware.*    -> 0x08040000

openmv.*      -> 0x08000000

Openmv.* for the combined firmware of bootloader and firmware, you can choose burning as needed

# QSPI Flash
The firmware is placed on the external 8MB QSPI Flash, so download 0x080000000.hex, initialize the QSPI peripheral, and map QSPI Flash to 0x90000000.
## firmware download address definition
0x08000000.hex -> 0x08000000

0x90000000.bin -> 0x90000000

> 0x90000000.bin firmware needs to use STM32CubeProg software, check enable QSPI Flash download algorithm, use ST-Link to download

> Bin file need to define the Download address, please note that STM32CubeProg `Download` button on the right side of the down `arrow`, click to customize the Download address

