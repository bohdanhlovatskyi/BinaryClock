# Importing Libraries
import serial
import serial.tools.list_ports
import time

for port in serial.tools.list_ports.comports():
    if "Serial" in port.description:
        arduino = serial.Serial(port=port.device, baudrate=9600, timeout=.1, rtscts=False, dsrdtr=False)
        date = input("Enter time: ")
        arduino.write(bytes('s'+date, 'utf-8'))
        time.sleep(0.05)
