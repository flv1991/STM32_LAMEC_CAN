#!/usr/bin/env python

# import the serial module so we can access the serial port
import serial 
import time
# set up serial port


# open file object in write mode
dataFile = open('data.dat', 'w')
dataFile2 = open('data2.dat', 'w') 
numberSamples = 50
 
# get specified number of samples
for i in range(numberSamples):
	serialPort0 = serial.Serial('/dev/ttyUSB0', 9600)
	serialPort0.write("x")
	reading0 = serialPort0.readline()
	print(reading0)
	dataFile.write(str(i)+ str(" ") + str(reading0)) # write sample number and reading to the file
	
	# close serial port to free it for other applications
	serialPort0.close()

	serialPort0 = serial.Serial('/dev/ttyUSB0', 9600)
	serialPort0.write("y")
	reading0 = serialPort0.readline()
	print(reading0)
	dataFile2.write(str(i)+ str(" ") + str(reading0)) # write sample number and reading to the file
	
	# close serial port to free it for other applications
	serialPort0.close()


# close serial port to free it for other applications
serialPort0.close()
