from rplidar import RPLidar
import numpy as np
import math
import matplotlib.pyplot as plt

port = RPLidar('COM7')
print(port.get_info())
print(port.get_health())

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
    ax.fill(deg, dist, 'r', alpha=0.5)
    plt.pause(0.001)
    return line,

predeg = []
predist = []

for scan in port.iter_scans(scan_type='express'):
    deg, dist, avgdeg, avgdist = [], [], [], []

    # if predeg == []:
    for i in scan:
        deg.append(math.radians(i[1]))
        predeg.append(math.radians(i[1]))
        dist.append(i[2])
        predist.append(i[2])
    line, = update(deg, dist, line)

    # Attempt at averaging w/ previous scan

    # else:
    #     for i in scan:
    #         deg.append(math.radians(i[1]))
    #         dist.append(i[2])
    #         for i in range(0, len(deg)):
    #             try: avgdeg.append((predeg[i] + deg[i])/2)
    #             except IndexError: avgdeg.append(deg[i])
    #             try: avgdist.append((predist[i] + dist[i])/2)
    #             except IndexError: avgdist.append(dist[i])
    #     line, = update(avgdeg, avgdist, line)

    # print(deg, dist)
