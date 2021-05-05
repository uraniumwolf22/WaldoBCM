import serial

port = serial.Serial('COM5', baudrate=115200)
port.read_timeout = 1

port.write('SADF'.encode())
response = port.read_until('X'.encode())
print(response)
