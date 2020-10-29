# WeAct Studio STM32H7xx

1. Example -> 例程
2. Firmwares -> openmv固件，内有烧录下载教程
3. Ports -> WeAct Studio STM32H750 核心板接口定义

## 接口定义 Interface Definition
> 具体接口定义(Concrete interface definition) `Ports\micropython\boards\WeActStudioSTM32H7xx\mpconfigboard.h`
```
代号，实际管脚名
Code name, Actual pin name
P0,PB15
P1,PB14
P2,PB13
P3,PB12
P4,PB10
P5,PB11
P6,PA5
P7,PD12
P8,PD13
P9,PD14
P10,PD15
P11,PA13
P12,PA14
P13,PA0
P14,PA1
P15,PA2
P16,PA3
LED_BLUE,PE3
```

## How to Build OpenMV

1. Clone openmv source code
```
git clone https://github.com/WeActTC/openmv.git --depth=1
```

2. Init source code
```
cd openmv
git submodule init
git submodule update --depth=1
cd src/micropython
git submodule init
git submodule update --depth=1
cd ..
```

3. Install gcc-arm-none-eabi
```
sudo apt-get install gcc-arm-none-eabi
```

4. Bulid
```
# 1. Copy `Ports/omv/boards/WeActStudioSTM32H7xx` to `openmv/src/omv/boards/`
# 2. Copy `Ports/micropython/boards/WeActStudioSTM32H7xx` to `openmv/src/micropython/ports/stm32/boards/`
# 3. Copy `Ports/micropython/micropython-Add-WeAct-Studio-STM32H7xx-Support.patch` to `openmv/src/micropython/`
# go to `openmv/src`
cd micropython
git am micropython-Add-WeAct-Studio-STM32H7xx-Support.patch
cd mpy-cross
make -j
cd ../../
make TARGET=WeActStudioSTM32H7xx -j
```

> bootloader.bin 0x08000000

> firmware.bin 0x08040000

> opemmv.bin 0x08000000