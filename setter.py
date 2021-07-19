# Importing Libraries
import serial
import time

arduino = serial.Serial(port='/dev/ttyUSB1', baudrate=9600, timeout=.1, rtscts=False, dsrdtr=False)
com = serial.Serial()
com.port = '/dev/ttyUSB1'
com.baudrate = 9600 
com.timeout = .1
com.setDTR(False)
com.open()
def write(date):
    arduino.write(bytes('s'+date, 'utf-8'))
    time.sleep(0.05)

date = input("Enter time: ")
value = write(date)

