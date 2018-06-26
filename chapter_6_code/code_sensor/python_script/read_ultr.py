#!/usr/bin/env python

import serial
import sys


try:
	ser = serial.Serial('/dev/ttyACM0',115200)
	

except:
	print "Unable to open serial port"

while True:

	try:
		line = ser.readline()
		print line
	except:
		print "Unable to read from device"

		sys.exit(0)	


