# This work is licensed under the MIT license.
# Copyright (c) 2013-2025 OpenMV LLC. All rights reserved.
# https://github.com/openmv/openmv/blob/master/LICENSE
#
# LCD Shield Example
#
# Note: To run this example you will need a LCD Shield for your OpenMV Cam.
#
# The LCD shield allows you to view your OpenMV Cam's frame buffer on the go.

import sensor
import time
import display
import image
from machine import Pin
import pyb

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)

lcd = display.SPIDisplay(vflip=True, hmirror=True, bgr=True, width = 80, height = 160)

lcd.bus_write(0x21)
xPos = 26
yPos = 1
lcd.bus_write(cmd=0x2A, args=bytearray([0, xPos & 0xff, 0, xPos+80-1]))
lcd.bus_write(cmd=0x2B, args=bytearray([0, yPos & 0xff, 0, yPos+160-1]))
lcd.bus_write(0x2C)
lcd.bus_write(cmd=0xC0, args=bytearray([0xAB, 0x0B]))
lcd.bus_write(cmd=0xC1, args=bytearray([0xC5]))
lcd.bus_write(cmd=0xC2, args=bytearray([0x0D, 0x00]))
lcd.bus_write(cmd=0xC4, args=bytearray([0x8d, 0xee]))
lcd.bus_write(cmd=0xC5, args=bytearray([0x0F]))
lcd.bus_write(cmd=0xE0, args=bytearray([0x07, 0x0E, 0x08, 0x07, 0x10, 0x07, 0x02,
                                        0x07, 0x09, 0x0F, 0x25, 0x36, 0x00, 0x08, 0x04, 0x10]))
lcd.bus_write(cmd=0xE1, args=bytearray([0x0A, 0x0D, 0x08, 0x07, 0x0F, 0x07, 0x02,
                                        0x07, 0x09, 0x0F, 0x25, 0x35, 0x00, 0x09, 0x04, 0x10]))

lcd.backlight(0)

# lcd_led = Pin('E10', Pin.OUT, Pin.PULL_NONE)
# lcd_led.value(1)
# Lcd_LEDCount = 0
# LCD_LEDSet = 1


# def LCD_LEDCtrl(timer):
#     global Lcd_LEDCount, LCD_LEDSet
#     Lcd_LEDCount = Lcd_LEDCount+1
#     if Lcd_LEDCount == 10:
#         Lcd_LEDCount = 0
#     if LCD_LEDSet > Lcd_LEDCount:
#         lcd_led.value(0)
#     else:
#         lcd_led.value(1)


# lcd_led_tim = pyb.Timer(2)
# lcd_led_tim.init(freq=4000)  # Freq: 4KHz
# lcd_led_tim.callback(LCD_LEDCtrl)

clock = time.clock()

while True:
    clock.tick()
    lcd.write(sensor.snapshot(), hint=image.CENTER | image.SCALE_ASPECT_KEEP | image.ROTATE_90)
    print(clock.fps())
