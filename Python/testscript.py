import serial

port = serial.Serial('COM3', baudrate=9600)
port.read_timeout = 1

port.write('SAAF'.encode())
response = port.read_until('X'.encode())
print(response)
