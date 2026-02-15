# 中文说明
我们提供两种不同地址运行的固件：Internal Flash，openmv在MCU内部存储器上取址运行，速度略快；QSPI Flash，openmv在MCU QSPI外设上取址运行，速度稍微慢点。

## Internal Flash
> Openmv 固件放在MCU内部存储地址，从0x08000000开始

### 烧录方法
固件下载需要使用USB模式下载，ST-Link下载会有大小限制，可以使用`WeAct Studio USB Download Tool`（位于Soft文件夹）或者使用STM32CubeProg进行下载，建议使用`WeAct Studio USB Download Tool`烧录openmv.bin文件，百度网盘有下载烧录视频教程
```
>> 下载烧录视频教程
百度网盘: https://pan.baidu.com/s/1wPy3f_cRzPJc_YnOVKTprA 
提取码: rji7
```
[STM32 下载烧录教程以及问题汇总](http://www.weact-tc.cn/2019/11/30/STM32Download/)

## QSPI Flash
> Openmv 固件放在外部8MB QSPI Flash，所以需要先下载0x08000000.hex，初始化QSPI外设，将QSPI Flash映射到0x90000000，具体见文件夹README.md。

### 烧录方法
* 方法1：使用STM32CubeProg软件，勾选使能QSPI Flash下载算法，使用ST-Link进行下载
* > Bin文件需要自己定义下载地址，请注意，STM32CubeProg `Download`按钮右边有向下的`小箭头`，点击可以自定义下载地址

* 方法2：使用WeAct HID Flash下载
* 1. 按住K1键上电或复位，LED慢闪时松开K1键进入WeAct HID Flash，可以使用WeAct HID Flash 上位机烧录openmv app: 0x90000000.bin
* 2. 按住K1键上电或复位，LED慢闪，继续按住K1键，LED变为快闪，松开K1键进入DFU模式

# English Description
We provide two kinds of firmware running at different addresses:Internal Flash,OpenMV is addressed and run on MCU Internal memory,which is slightly faster;QSPI Flash,OpenMV runs on MCU QSPI peripherals at a slightly slower speed.

## Internal Flash
> The Openmv firmware download is at the internal address, starting at 0x08000000

> Firmware Download needs to use USB mode, st-link Download will have size limit, you can use  `WeAct Studio USB Download Tool` (located in `Soft` folder) or `STM32CubeProg` to Download, it is recommended to use `WeAct Studio USB Download Tool` to burn openmv.bin file

## QSPI Flash
> The Openmv firmware is placed on the external 8MB QSPI Flash, so download 0x080000000.hex, initialize the QSPI peripheral, and map QSPI Flash to 0x90000000.

Firmware needs to use STM32CubeProg software, check enable QSPI Flash download algorithm, use ST-Link to download

> Bin file need to define the Download address, please note that STM32CubeProg `Download` button on the right side of the down `arrow`, click to customize the Download address
