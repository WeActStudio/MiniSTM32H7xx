from pyb import UART
from pyb import LED
from time import sleep

uart1 = UART(1, 9600)  # PA9,PA10
uart1.write('hello')

while True:
    rec=uart1.read(50)
    if rec != None:
        print(rec)
        for t in rec:
            print('%#x '%t,end='')
        print('\r\n')
    sleep(0.1)
    LED(1).toggle()
