from rplidar import RPLidar
import numpy as np
import math
import matplotlib.pyplot as plt

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

port = RPLidar('COM5')

print(port.get_info())
print(port.get_health())

for scan in port.iter_scans(scan_type='express'):
    deg = []
    dist = []
    for i in scan:
        deg.append(math.radians(i[1]))
        dist.append(i[2])
    line, = update(deg, dist, line)
    print(deg, dist)
