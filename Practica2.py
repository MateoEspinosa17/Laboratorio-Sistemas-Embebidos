import time
import grovepi
from grovepi import *
from grove_rgb_lcd import *
import math

# SIG,NC,VCC,GND
sensor = 2
button = 7		#Port for Button

pinMode(button,"INPUT")		# Assign mode for Button as input
while True:
    try:
        buttonStatus = digitalRead(button)
        if buttonStatus :
            temp = math.floor(grovepi.temp(sensor,'1.1'))
            string = "Temp = " + str(temp) + " C"
            setRGB(0,255,0)
            setText(string)
            time.sleep(1.5)
        else:
            setText("Presiona el\nboton")
            setRGB(255,0,0) #color rojo
            time.sleep(1.5)
    except KeyboardInterrupt:
    	break
    except IOError:
    	print("Error")

    