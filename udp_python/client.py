import socket 
import sys

if len(sys.argv) < 3:
    print("Usage: "+sys.argv[0]+" server-ip port-numer")
    exit(0)

UDP_IP_ADDRESS = sys.argv[1]
UDP_PORT_NO = int(sys.argv[2])

message = input("Message: ")

# create udp socket
client_sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

while True:
    # encode bytes
    client_sock.sendto(message.encode(), (UDP_IP_ADDRESS, int(UDP_PORT_NO)))
    message = input("Message: ")