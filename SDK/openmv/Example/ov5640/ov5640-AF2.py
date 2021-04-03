# WeAct Studio
# 微行工作室
# OV5640 自动对焦测试
# 单击K1,启动单点自动对焦
# 长按K1,释放对焦马达,镜头回到初始状态

import sensor, image, time, lcd
from time import sleep
from pyb import LED,Pin

sensor.reset()
sensor.set_pixformat(sensor.RGB565)
sensor.set_framesize(sensor.QQVGA)
sensor.skip_frames(time = 20)

blue_led = LED(1)
KEY = Pin('C13',Pin.IN,Pin.PULL_DOWN)
lcd.init(type=2,refresh=120)

clock = time.clock()
keycount=0
while(True):
    clock.tick()
    img = sensor.snapshot()
    print(clock.fps())
    lcd.display(img)
    if sensor.get_id() == sensor.OV5640:
            if KEY.value() == 1:
                while KEY.value() == 1:
                    blue_led.on()
                    sleep(0.05)
                    blue_led.off()
                    sleep(0.05)
                    keycount += 1
                    if keycount > 3:
                        sensor.ioctl(sensor.IOCTL_RESET_AUTO_FOCUS)
                        while KEY.value() == 1:
                            blue_led.on()
                            sleep(0.1)
                            blue_led.off()
                            sleep(0.1)
                if keycount <= 3:
                    sensor.ioctl(sensor.IOCTL_TRIGGER_AUTO_FOCUS)
            if keycount != 0:
                sensor.ioctl(sensor.IOCTL_WAIT_ON_AUTO_FOCUS)
                keycount = 0
