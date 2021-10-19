import socket
from rplidar import RPLidar
import json

liport = RPLidar('COM5')
print(liport.get_info())
print(liport.get_health())

HOST = 'localhost'    # The remote host
PORT = 8000            # The same port as used by the server
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

for scan in liport.iter_scans(scan_type='express',max_buf_meas=1000000):
    # print(scan)
    s.sendall(json.dumps(scan).encode())
    # response = s.recv(1024)
    # print(response.decode())

s.close()