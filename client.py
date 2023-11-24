import socket


def __client(message : str):
    hostname = socket.gethostname()
    port = 6379
    sock = socket.socket()
    sock.connect((hostname, port))

    sock.send(message.encode())

    data = sock.recv(1024)
    data = data[:data.find(0)].decode()
 
    sock.close()

    return data

def client(query : str):
    return __client("client.py --file bd.txt --query " + query)