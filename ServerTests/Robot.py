from rplidar import RPLidar
import websockets
import asyncio
import json

liport = RPLidar('COM5')
print(liport.get_info())
print(liport.get_health())

async def main():
    async with websockets.connect('ws://rossnation.ddns.me:8000') as ws:
        for scan in liport.iter_scans(scan_type='express'):
            # print(scan)
            ws.send(json.dumps(scan).encode())
            # response = s.recv(1024)
            # print(response.decode())

asyncio.run(main())
