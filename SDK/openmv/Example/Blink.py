import pyb
import micropython
import time
micropython.alloc_emergency_exception_buf(100)


class Foo(object):
    def __init__(self, timer, led):
        self.led = led
        timer.callback(self.cb)

    def cb(self, tim):
        self.led.toggle()


blue = Foo(pyb.Timer(2, freq=2), pyb.LED(1))  # LED(1) -> PE3

while True:
    time.sleep(1)
