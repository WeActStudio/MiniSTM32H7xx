V4.8.1+相较于V4.4.1之前的版本改动较大，mpy函数也有变化，差异点如下：  
1. 不支持spi flash以及qspi flash
2. 存储代码需要使用TF卡
3. LCD屏幕显示代码变动较大，请参考Example/lcd_0.96.py

V4.8.1+ has undergone significant changes compared to the previous V4.4.1. The mpy function has also been modified. The differences are as follows:
1. SPI flash and QSPI flash are not supported.
2. The storage code requires the use of a TF card.
3. The display code on the LCD screen has undergone significant changes. Please refer to Example/lcd_0.96.py for details.

# 怎样烧录固件 How to Burn Firmware
使用Cubeprogrammer烧录openmv_0x08000000.bin，地址为0x08000000，重新上电即可  
Use Cubeprogrammer to burn openmv_0x08000000.bin, address is 0x08000000, then power on again.