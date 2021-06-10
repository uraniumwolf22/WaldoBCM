import socket
import matplotlib.pyplot as plt
import math
import json
import base64

plt.style.use('ggplot')
plt.ion()

deg = []
dist = []
fig, ax = plt.subplots(subplot_kw={'projection': 'polar'})
line, = ax.plot(deg, dist)
plt.title('Spinny Laser')
plt.show()

def update(deg, dist, line):
    plt.cla()
    line.set_data(deg, dist)
    fig.gca().relim()
    fig.gca().autoscale_view()
    ax.fill(deg, dist, alpha=0.5)
    plt.pause(0.001)
    return line,


HOST = '0.0.0.0'         # Symbolic name meaning all available interfaces
PORT = 8000              # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
conn, addr = s.accept()
print('Connected by', addr)

while 1:
    data = conn.recv(16384)
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
    #print(deg, dist)
# print(data.decode())
conn.close()
