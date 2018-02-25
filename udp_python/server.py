import socket 
import sys

if len(sys.argv) < 3:
    print("Usage: "+sys.argv[0]+" ip port-numer")
    exit(0)

UDP_IP_ADDRESS = sys.argv[1]
UDP_PORT_NO = int(sys.argv[2])

server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# unlike client.py, this requires binding 
# binding sock with declared ip and port number
server_socket.bind((UDP_IP_ADDRESS, UDP_PORT_NO))
print("Listening on port "+str(UDP_PORT_NO))
while True:
    message, client_addr = server_socket.recvfrom(1024)
    # decode bytes
    print("Message: \'"+str(message.decode())+"\' received from "+str(client_addr))
