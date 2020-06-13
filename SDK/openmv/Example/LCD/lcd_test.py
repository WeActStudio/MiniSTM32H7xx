# WeAct Studio
# 微行工作室
# TFT ST7735 Test

import sensor, image, time
from ST7735 import TFT,TFTColor
from sysfont import sysfont
from machine import Pin, SPI
import pyb,time

spi = SPI(-1, baudrate=20000000, polarity=0, phase=0, sck=Pin('E12'), mosi=Pin('E14'), miso=Pin('A0'))
# DC       - RS/DC data/command flag
# CS       - Chip Select, enable communication
# RST/RES  - Reset
dc  = Pin('E13', Pin.OUT, Pin.PULL_NONE)
cs  = Pin('E11', Pin.OUT, Pin.PULL_NONE)
rst = Pin('E15', Pin.OUT, Pin.PULL_NONE)

lcd_led = Pin('E10', Pin.OUT, Pin.PULL_NONE)
lcd_led.value(1)
Lcd_LEDCount=0
LCD_LEDSet=0
def LCD_LEDCtrl(timer):
    global Lcd_LEDCount,LCD_LEDSet
    Lcd_LEDCount=Lcd_LEDCount+1
    if Lcd_LEDCount == 10:
        Lcd_LEDCount=0
    if LCD_LEDSet > Lcd_LEDCount: lcd_led.value(0)
    else: lcd_led.value(1)

lcd_led_tim = pyb.Timer(2)
lcd_led_tim.init(freq=4000) # Freq: 4KHz
lcd_led_tim.callback(LCD_LEDCtrl)

def LCD_ShowBmp(_tft,FileName):
    f=open(FileName, 'rb')
    print(FileName)
    if f.read(2) == b'BM':  #header
        dummy = f.read(8) #file size(4), creator bytes(4)
        offset = int.from_bytes(f.read(4), 'little')
        hdrsize = int.from_bytes(f.read(4), 'little')
        width = int.from_bytes(f.read(4), 'little')
        height = int.from_bytes(f.read(4), 'little')
        if int.from_bytes(f.read(2), 'little') == 1: #planes must be 1
            depth = int.from_bytes(f.read(2), 'little')
            if depth == 24 and int.from_bytes(f.read(4), 'little') == 0:#compress method == uncompressed
                print("Image size:", width, "x", height)
                rowsize = (width * 3 + 3) & ~3
                if height < 0:
                    height = -height
                    flip = False
                else:
                    flip = True
                w, h = width, height
                if w > 160: w = 60
                if h > 80: h = 80
                tft._setwindowloc((0,0),(w - 1,h - 1))
                for row in range(h):
                    if flip:
                        pos = offset + (height - 1 - row) * rowsize
                    else:
                        pos = offset + row * rowsize
                    if f.tell() != pos:
                        dummy = f.seek(pos)
                    for col in range(w):
                        bgr = f.read(3)
                        _tft._pushcolor(TFTColor(bgr[2],bgr[1],bgr[0]))
            else:
                print(FileName+'is not 24bit pic')

tft=TFT(spi,dc,rst,cs)
tft.init_7735(TFT.GREENTAB80x160)
tft.rotation(3)
LCD_ShowBmp(tft,'WeActStudiologo.bmp')
time.sleep(50)
LCD_LEDSet=1
time.sleep(1000)
tft.fill(TFT.BLACK)
tft.text((0, 30), 'Hello WeAct!', TFT.WHITE, sysfont, 2, nowrap=False)

sensor.reset()                      # Reset and initialize the sensor.
sensor.set_pixformat(sensor.RGB565) # Set pixel format to RGB565 (or GRAYSCALE)
sensor.set_framesize(sensor.QVGA)   # Set frame size to QVGA (320x240)

if sensor.get_id() == sensor.OV7725:
    sensor.set_hmirror(True)
    sensor.set_vflip(True)

sensor.skip_frames(time = 1000)     # Wait for settings take effect.
clock = time.clock()                # Create a clock object to track the FPS.

while(True):

    clock.tick()                    # Update the FPS clock.
    img = sensor.snapshot()         # Take a picture and return the image.
    print(clock.fps())              # Note: OpenMV Cam runs about half as fast when connected
                                    # to the IDE. The FPS should increase once disconnected.
    tft.image(0,0,160-1,80-1,img.copy([80,80,160,80]))
