import sys
import socket

from _thread import *
import threading

conns = []
serverRunning = False
serverSock = 0

def threaded(c):
    global conns
    global serverRunning
    while serverRunning:
        data = c.recv(1024)
        if not data:
            print('Bye')
            break
        print(data)
    c.close() 
    conns.remove(c)

def sockListen():
    global serverSock
    global serverRunning
    global conns

    while True:
        if not serverRunning: break
        
        (clientSock, address) = serverSock.accept()

        if not serverRunning: break
        print("Got client ", address[0], ":", address[1])
        conns.append(clientSock)

        with clientSock:
            print(address, ' connected.')
            s = "Hello World!"
            byte_str = s.encode()
            clientSock.send(byte_str)
            #start_new_thread(threaded, (clientSock,)) TODO Handle any robot responses
    print("ended")

serverThread = threading.Thread(target = sockListen)

def startServer():
    global serverSock
    global serverRunning
    global serverThread
    HOST = ""
    PORT = 8888

    serverSock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serverSock.bind((HOST, PORT))
    serverSock.listen(5)
    print("Server created")

    serverRunning = True
    serverThread.start()


def serverClose():
    global serverSock
    global serverRunning
    global serverThread
    global conns

    serverRunning = False

    for sock in conns:
        print("closing ", sock)
        sock.close()
        conns.remove(sock)

    serverSock.close()
    serverSock.stop()
    serverThread.join()

def main():
    global serverSock
    global serverRunning
    while 1:
        cmd = input("> ")
        if cmd == 'start server':
            startServer()
        elif cmd == 'close server':
            serverClose()
        elif cmd == 'connect':
            ip = input('Server IP: ')
            port = input('Server Port: ')

            sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
            address = (ip, port)

            msg = "It Works!"

            sock.sendto(bytes(msg, "utf-8"), address)
            sock.close()
        else:
            words = cmd.split(" ")
            if words[0] == 1: # send to the first client 
                byte_str = cmd.encode()
                conns[0].send(cmd)

if __name__ == '__main__':
    main()