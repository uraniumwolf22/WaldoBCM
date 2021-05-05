import serial

port = serial.Serial('COM3', baudrate=9600)

port.write('SAAF'.encode())
print(port.read())
