import socket
import json
import math
HOST = 'localhost'
PORT = 8001
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

while 1:
    data = s.recv(16384)
    if not data: break
    # conn.sendto('success'.encode(), addr)
    try: scan = json.loads(data.decode())
    except json.decoder.JSONDecodeError: continue

    deg = []
    dist = []
    for i in scan:
        deg.append(math.radians(i[1]))
        dist.append(i[2])
    line, = update(deg, dist, line)
    print(deg,dist)