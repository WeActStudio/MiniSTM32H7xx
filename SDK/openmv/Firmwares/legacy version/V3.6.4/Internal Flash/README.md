# 中文说明
## Internal Flash Firmware
> Openmv 固件放在内部地址，从0x08000000开始
### 8MB QSPI Flash
> 8MB QSPI Flash 作为Python代码存储器
### 8MB SPI Flash (推荐使用该文件夹下的固件)
> 8MB SPI Flash 作为Python代码存储器
### 固件下载地址定义
* bootloader.*  -> 0x08000000

* firmware.*    -> 0x08040000

* openmv.*      -> 0x08000000

openmv.* 为bootloader和firmware两个固件的合并固件，按需选择烧录即可

> 固件下载需要使用USB模式下载，ST-Link下载会有大小限制，可以使用`WeAct Studio USB Download Tool`（位于Soft文件夹）或者使用STM32CubeProg进行下载，建议使用`WeAct Studio USB Download Tool`烧录openmv.bin文件，百度网盘有下载烧录视频教程

```
>> 下载烧录视频教程
百度网盘: https://pan.baidu.com/s/1wPy3f_cRzPJc_YnOVKTprA 
提取码: rji7
```

# English Description
## Internal Flash Firmware
The firmware download is at the internal address, starting at 0x08000000
### 8 MB QSPI Flash
8MB QSPI Flash as Python code memory
### 8 MB SPI Flash
8MB SPI Flash as Python code memory
### firmware download address definition
bootloader.*  -> 0x08000000

firmware.*    -> 0x08040000

openmv.*      -> 0x08000000

Openmv.* for the combined firmware of bootloader and firmware, you can choose burning as needed

> Firmware Download needs to use USB mode, st-link Download will have size limit, you can use  `WeAct Studio USB Download Tool` (located in `Soft` folder) or `STM32CubeProg` to Download, it is recommended to use `WeAct Studio USB Download Tool` to burn openmv.bin file