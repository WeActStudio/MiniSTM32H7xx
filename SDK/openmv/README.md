# WeAct Studio STM32H7xx
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
# Copy `Ports/omv/boards/WeActStudioSTM32H7xx` to `openmv/src/omv/boards/`
# Copy `Ports/micropython/boards/WeActStudioSTM32H7xx` to `openmv/src/micropython/ports/stm32/boards/`
# go to `openmv/src`
cd micropython/mpy-cross
make -j
cd ../../
make TARGET=WeActStudioSTM32H7xx -j
```

> bootloader.bin 0x08000000

> firmware.bin 0x08040000

> opemmv.bin 0x08000000