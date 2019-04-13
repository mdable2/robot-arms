import sys
import socket

from _thread import *
import threading

conns = []
serverRunning = False

def threaded(c):
    while True:
        data = c.recv(1024)
        if not data:
            print('Bye')
            break
        print(data)
    c.close()
    global conns 
    conns.remove(c)

def sockListen():
    print("Waiting")
    while True:
        global serverRunning
        if not serverRunning: break

        (clientSock, address) = server.accept()
        global conns
        conns.append(clientSock)

        with clientSock:
            print(address, ' connected.')
            s = "Hello World!"
            byte_str = s.encode()
            clientSock.send(byte_str)
            start_new_thread(threaded, (clientSock,))
    #serverClose()

serverThread = threading.Thread(target = sockListen)
serverSock = 0

def startServer():
    HOST = ""
    PORT = 8888

    serverSock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serverSock.bind((HOST, PORT))
    serverSock.listen(5)
    print("Server created")

    serverRunning = True
    serverThread.start()


def serverClose():
    serverRunning = False
    for sock in conns:
        sock.close()
        conns.remove(sock)

    serverThread.join()
    serverSock.close()

def main():
    while 1:
        cmd = input("> ")
        if cmd == 'start server':
            startServer()
        elif cmd == 'close server':
            serverRunning = 0;
            serverClose()
            serverThread.join()
        else:
            words = cmd.split(" ")
            if words[0] == 1:
                conns[0].send(cmd)

if __name__ == '__main__':
    main()