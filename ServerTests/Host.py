import websockets
import asyncio
import math
import json

global packet
packet = [[.5],[1000]]

async def recv(ws, path):
    while True:
        print("SCAN RESET")
        data = await ws.recv()
        if not data: break
        try: scan = json.loads(data.decode())
        except json.decoder.JSONDecodeError: continue

        deg = []
        dist = []
        for i in scan:
            deg.append((math.radians(i[1])) * 180 / math.pi)
            dist.append((i[2]))
        print(deg, dist)

        global packet
        packet = [deg,dist]

        x = []
        y = []
        for pair in range(len(deg)):
            x.append(dist[pair] * math.cos(deg[pair]))
            y.append(dist[pair] * math.sin(deg[pair]))
            print("X = ", x[pair])
            print("Y = ", y[pair])

async def send(ws, path):
    message = await ws.recv()
    print(json.loads(message))

    global packet
    if packet:
        while True:
            global packet
            await ws.send(json.dumps(packet))
            await asyncio.sleep(0.001)


if __name__ == "__main__":
    server_bot = websockets.serve(recv, "0.0.0.0", 8000)
    server_web = websockets.serve(send, "0.0.0.0", 8001)

    asyncio.get_event_loop().run_until_complete(server_bot)
    asyncio.get_event_loop().run_until_complete(server_web)
    asyncio.get_event_loop().run_forever()
