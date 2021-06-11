import socket
import json


HOST = 'localhost'         # Symbolic name meaning all available interfaces
PORT = 8002              # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
conn, addr = s.accept()
print('Connected by', addr)

while True:
    data = conn.recv(1024)
    try: data = json.loads(data.decode())
    except json.decoder.JSONDecodeError: continue
    print(data)