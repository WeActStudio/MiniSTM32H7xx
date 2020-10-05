# LCD Example
#
# Note: To run this example you will need a 0.96'' LCD for your Board.
#
# WeAct Studio STM32H7xx

import sensor, image, lcd, time

sensor.reset()                      # Initialize the camera sensor.
sensor.set_pixformat(sensor.RGB565) # or sensor.GRAYSCALE
sensor.set_framesize(sensor.QQVGA)  # Special 160x120 framesize for 0.96'' LCD.
lcd.init(type=2)                    # Initialize the lcd screen.
clock = time.clock()                # Create a clock object to track the FPS.

while(True):
    clock.tick()                    # Update the FPS clock.
    lcd.display(sensor.snapshot())  # Take a picture and display the image.
    print(clock.fps())              # Note: OpenMV Cam runs about half as fast when connected
                                    # to the IDE. The FPS should increase once disconnected.
