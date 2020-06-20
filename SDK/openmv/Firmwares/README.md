# SPI Flash Erase Firmware
由于openmv固件是没有办法擦除外部的SPI存储器，所以我们提供`SPI Flash Erase Firmware`固件用于擦除外部的两颗`SPI Flash`
* `SPI_Flash_Erase_0x8000000.hex`可以使用我们提供的`WeAct Studio USB Download Tool`进行下载，下载完成后，等待程序运行完成，核心板屏幕显示`Please Burn Next Fiwmware`后可以重新烧录固件
* `SPI_Flash_Erase_0x8040000.bin`可以直接使用openmv IDE进行下载，具体方法为：
1. 选择`工具->运行引导加载程序(加载固件)`
2. 选择`SPI_Flash_Erase_0x8040000.bin`,然后点击运行即可
3. 下载完成后，等待程序运行完成，核心板屏幕显示`Please Burn Next Fiwmware`后继续下一步
4. openmv IDE 继续勾选`工具->运行引导加载程序(加载固件)`,选择我们资料提供的`firmware.bin`固件(不是USB Download软件里面的固件)，然后点击运行,之后核心板单击复位按钮，等待固件下载完成

# V3.6.4

* 具体详情见文件夹内的README.md
