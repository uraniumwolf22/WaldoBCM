import socket

def server():
  host = "10.1.10.185"   # get local machine name
  port = 8080  # Make sure it's within the > 1024 $$ <65535 range
  
  s = socket.socket()
  s.bind((host, port))
  
  s.listen(1)
  client_socket, adress = s.accept()
  print("Connection from: " + str(addr))
  while True:
    data = c.recv(1024).decode('utf-8')
    if not data:
      break
    print('From online user: ' + data)
    data = data.upper()
    c.send(data.encode('utf-8'))
  c.close()

if __name__ == '__main__':
server()