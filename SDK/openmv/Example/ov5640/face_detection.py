# WeAct Studio
# 微行工作室

import sensor, time, image, sys

from time import sleep

from pyb import LED,Pin

# OV5640 AF 固件初始化
def OV5640AF_Init():
    AFConfigFile = open('AF_REG.txt','r')
    addr=0x8000
    sensor.__write_reg(0x3000,0x20)
    while True:
        AFConfig=AFConfigFile.readline()
        if not AFConfig:
            break
        str_new = AFConfig.replace(' ', '')
        str_new = str_new.replace(',', '')
        str_new = str_new.replace('0x', '')
        str_new = str_new[:-8]
        len_s = int(len(str_new)/2)
        list_nums = []
        i = 0
        for i in range(0, len_s):
            chs = str_new[2*i : 2*i + 2]
            num = int(chs, 16)
            list_nums.append(num)
        bys = bytes(list_nums)
        # print(bys)
        for char in bys:
            sensor.__write_reg(addr,char)
            addr += 1
    sensor.__write_reg(0x3022,0x00)
    sensor.__write_reg(0x3023,0x00)
    sensor.__write_reg(0x3024,0x00)
    sensor.__write_reg(0x3025,0x00)
    sensor.__write_reg(0x3026,0x00)
    sensor.__write_reg(0x3027,0x00)
    sensor.__write_reg(0x3028,0x00)
    sensor.__write_reg(0x3029,0x7f)
    sensor.__write_reg(0x3000,0x00)

    while(True):
        result = sensor.__read_reg(0x3029)
        print('FW_STATUS: %X' %result)
        if result != 0x7F:
            break
        sleep(500)

# Reset sensor
sensor.reset()

# Sensor settings
sensor.set_contrast(3)
sensor.set_gainceiling(16)
# HQVGA and GRAYSCALE are the best for face tracking.
sensor.set_framesize(sensor.HQVGA)
sensor.set_pixformat(sensor.GRAYSCALE)

if sensor.get_id() != sensor.OV5640:
    print('Only run for ov5640')
    sys.exit(0)

OV5640AF_Init()

blue_led = LED(1)
KEY = Pin('C13',Pin.IN,Pin.PULL_DOWN)

# Load Haar Cascade
# By default this will use all stages, lower satges is faster but less accurate.
face_cascade = image.HaarCascade("frontalface", stages=25)
print(face_cascade)

# FPS clock
clock = time.clock()
keycount=0
while (True):
    clock.tick()

    # Capture snapshot
    img = sensor.snapshot()

    # Find objects.
    # Note: Lower scale factor scales-down the image more and detects smaller objects.
    # Higher threshold results in a higher detection rate, with more false positives.
    objects = img.find_features(face_cascade, threshold=0.75, scale_factor=1.25)

    # Draw objects
    for r in objects:
        img.draw_rectangle(r)

    # Print FPS.
    # Note: Actual FPS is higher, streaming the FB makes it slower.
    print(clock.fps())

    if KEY.value() == 1:
        while KEY.value() == 1:
            blue_led.on()
            sleep(0.05)
            blue_led.off()
            sleep(0.05)
            keycount += 1
            if keycount > 3:
                # 长按K1,释放对焦马达,镜头回到初始状态
                sensor.__write_reg(0x3022,0x08)
                while KEY.value() == 1:
                    blue_led.on()
                    sleep(0.1)
                    blue_led.off()
                    sleep(0.1)
        if keycount <= 3:
            sensor.__write_reg(0x3022,0x03)

    # 判断对焦是否完成
    if keycount != 0:
        # 读取对焦状态
        result = sensor.__read_reg(0x3029)
        print('FW_STATUS: %X' %result)
        if result == 0x10 or result == 0x70:
            # 对焦完成,暂停对焦过程,使镜头将保持在此对焦位置
            sensor.__write_reg(0x3022,0x06)
            keycount = 0
