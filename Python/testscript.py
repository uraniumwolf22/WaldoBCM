import serial
import time
port = serial.Serial('COM5', baudrate=115200)
time.sleep(3)

port.write('SAAF'.encode())
port.read_until('X'.encode())
print("IT FRIKKEN WORKED")