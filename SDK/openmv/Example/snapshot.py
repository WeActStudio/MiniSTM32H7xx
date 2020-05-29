# WeAct Studio
# 微行工作室

import sensor, image, pyb
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
            break;
        sleep(500);


sensor.reset() # Initialize the camera sensor.
sensor.set_pixformat(sensor.RGB565) # or sensor.GRAYSCALE
sensor.set_framesize(sensor.QVGA) # or sensor.QQVGA (or others)

if sensor.get_id() == sensor.OV7725:
    sensor.set_hmirror(True)
    sensor.set_vflip(True)
elif sensor.get_id() == sensor.OV5640:
    OV5640AF_Init()

sensor.skip_frames(time = 2000) # Let new settings take affect.

blue_led = LED(1)
KEY = Pin('C13',Pin.IN,Pin.PULL_DOWN)

print("You're on camera!")
keycount=0
file_count=0
while(True):
    sensor.snapshot()
    if KEY.value() == 1:
        while KEY.value() == 1:
            blue_led.on()
            sleep(50)
            blue_led.off()
            sleep(50)
            keycount += 1
            if keycount > 3:
                # 长按K1,开始对焦
                if sensor.get_id() == sensor.OV5640:
                    while KEY.value() == 1:
                        blue_led.on()
                        sleep(100)
                        blue_led.off()
                        sleep(100)
                    sensor.__write_reg(0x3022,0x03)

        if keycount <= 3:
            sleep(100)
            name='example'+str(file_count)+'.jpg'
            sensor.snapshot().save(name)
            print("Done! Reset the camera to see the saved image.")
            print(name)
            file_count += 1
            if sensor.get_id() != sensor.OV5640:
                keycount = 0

    if sensor.get_id() == sensor.OV5640:
        # 判断对焦是否完成
        if keycount != 0:
            # 读取对焦状态
            result = sensor.__read_reg(0x3029)
            print('FW_STATUS: %X' %result)
            if result == 0x10 or result == 0x70:
                # 对焦完成,暂停对焦过程,使镜头将保持在此对焦位置
                sensor.__write_reg(0x3022,0x06)
                keycount = 0

    else:
        keycount = 0
