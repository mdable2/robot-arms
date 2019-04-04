import socket

HOST = '192.168.43.235'  # Standard loopback interface address (localhost)
PORT = 65432        # Port to listen on (non-privileged ports are > 1023)
conn = []

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen(2)
    for i in range (0,2):
        c, addr = s.accept()
        conn.append(c)
        with conn[i]:
            print('Connected by', addr)
            msg = "Hello World!"
            byte_str = msg.encode()
                # data = conn.recv(1024)
                # if not data:
                #     break
            conn[i].send(byte_str)