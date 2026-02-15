# 中文说明
## QSPI Flash Firmware
Openmv 固件放在外部8MB QSPI Flash，所以需要下载0x08000000.hex，初始化QSPI外设，将QSPI Flash映射到0x90000000。
### 固件下载地址定义
0x08000000.hex -> 0x08000000

0x90000000.bin -> 0x90000000

### 烧录方法
1. 使用`STM32CubeProg`或者`WeAct Studio USB Download Tool`烧录0x08000000.hex
2. 使用WeAct HID Flash烧录0x90000000.bin

### 0x08000000.hex 说明
1. 按住K1键上电或复位，LED慢闪时松开K1键进入WeAct HID Flash，可以使用WeAct HID Flash 上位机烧录openmv app: 0x90000000.bin
2. 按住K1键上电或复位，LED慢闪，继续按住K1键，LED变为快闪，松开K1键进入DFU模式

# English Description

## QSPI Flash
The Openmv firmware is placed on the external 8MB QSPI Flash, so download 0x080000000.hex, initialize the QSPI peripheral, and map QSPI Flash to 0x90000000.

## firmware download address definition
0x08000000.hex -> 0x08000000
0x90000000.bin -> 0x90000000

### Burning method
1. Use `STM32CubeProg` or `WeAct Studio USB Download Tool` to burn 0x08000000.hex
2. Run WeAct HID Flash to burn 0x90000000.bin

### 0x08000000. Hex
1. Press and hold the K1 key to power on or reset, release the K1 key when the LED blinks slowly to enter the WeAct HID Flash, you can use the WeAct HID Flash upper computer to burn OpenMV App: 0x90000000.bin
2. Press and hold THE K1 button to power on or reset, and the LED blinks slowly. Continue to press and hold the K1 button, and the LED blinks quickly

