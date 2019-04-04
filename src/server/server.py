import socket

HOST = '192.168.43.235'  # Standard loopback interface address (localhost)
PORT = 65432        # Port to listen on (non-privileged ports are > 1023)

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print('Connected by', addr)
        s = "Hello World!"
        byte_str = s.encode()
            # data = conn.recv(1024)
            # if not data:
            #     break
        conn.send(byte_str)


# import http.server
# import socketserver

# PORT = 8080
# Handler = http.server.SimpleHTTPRequestHandler

# with socketserver.TCPServer(("", PORT), Handler) as httpd:
#     print("serving at port", PORT)
#     httpd.serve_forever()

#### Adafruit IP:
# IP address: 
# 10.0.0.169
# Netmask: 255.255.255.0
# Gateway: 10.0.0.1


#### My IP: 
# 10.0.0.82