import socket
import os


class Server:

    def __init__(self, port_nr =80) -> None:
            
        self.host = "127.0.0.1"
        self.port_nr = port_nr

        self.server_sc = socket.socket()
        self.server_sc.bind((self.host, self.port_nr))

    

    def connect(self):

        self.server_sc.listen(5)
        
        while True:
            
            c, addr = self.server_sc.accept()    
            print ('Got connection from', addr )
            data = c.recv(1024).decode()
            print(data)
            
            
server = Server(8080)
server.connect()


