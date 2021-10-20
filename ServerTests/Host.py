import socket
import matplotlib.pyplot as plt
import math
import json
import base64

HOST = '0.0.0.0'         # Symbolic name meaning all available interfaces
PORT = 8000              # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
conn, addr = s.accept()
print('Connected by', addr)

while 1:
    x = []
    y = []
    print("SCAN RESET")
    data = conn.recv(16384)
    if not data: break
    try: scan = json.loads(data.decode())
    except json.decoder.JSONDecodeError: continue

    deg = []
    dist = []


    for i in scan:
        deg.append((math.radians(i[1])) * 180 / math.pi )
        dist.append((i[2]))
    # print(deg, dist)
    # print("deg = ",deg)

    for pair in range(len(deg)):
        if deg[pair] == 0:
            x.append(0)
            y.append(dist[pair])

        elif deg[pair] == 90:
            x.append(dist(pair))
            y.append(0)

        elif deg[pair] == 180:
            x.append(0)
            y.append(0 - dist[pair])

        elif deg[pair] == 270:
            x.append(0 - dist[pair])
            y.append(0)


        elif deg[pair] > 0 and deg[pair] < 90:
            x.append(dist[pair] * math.cos(deg[pair]))
            y.append(dist[pair] * math.sin(deg[pair]))
            print("x = ", x[pair])
            print("y = ", y[pair])


        elif deg[pair] > 90 and deg[pair] < 180:
            x.append(dist[pair] * (math.sin(deg[pair] - 90)))
            y.append(0 - (dist[pair] * (math.cos(deg[pair] - 90))))
            print("x = ", x[pair])
            print("y = ", y[pair])            


        elif deg[pair] > 180 and deg[pair] < 270:
            x.append(0 - (dist[pair] * math.cos(deg[pair])))
            y.append(0 - (dist[pair] * math.sin(deg[pair])))
            print("x = ", x[pair])
            print("y = ", y[pair])

        elif deg[pair] > 270 and deg[pair] < 360:
            x.append(0 - (dist[pair] * (math.sin(deg[pair] - 90))))
            y.append(dist[pair] * (math.cos(deg[pair] - 90)))
            print("x = ", x[pair])
            print("y = ", y[pair])   

conn.close()