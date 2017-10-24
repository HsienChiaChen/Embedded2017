#!/usr/bin/python

import sys
from time import sleep
import Adafruit_DHT
import os

sensor = Adafruit_DHT.DHT11
pin = 4


while(1):
    humidity, temperature = Adafruit_DHT.read_retry(sensor, pin)

    if humidity is not None and temperature is not None:
        #print('Temp={0:0.1f}*  Humidity={1:0.1f}%'.format(temperature, humidity))
        
        cmd = "sudo gatttool -b B8:27:EB:5C:4C:AF --char-write-req -a 0x000c -n "
        cmd1 = cmd + str(temperature/100)
        os.system(cmd1)
        cmd2 = cmd + str(humidity/100)
        os.system(cmd2)

    else:
        print('Failed to get reading. Try again!')
        sys.exit(1)
    sleep(0.5)
