import socket 
import sys

if len(sys.argv) < 3:
    print("Usage: "+sys.argv[0]+" server-ip port-numer")
    exit(0)

UDP_IP_ADDRESS = sys.argv[1]
UDP_PORT_NO = int(sys.argv[2])

message = input("Message: ")

client_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

while True:
    client_sock.sendto(message, (UDP_IP_ADDRESS, UDP_PORT_NO))
    message = input("Message: ")
