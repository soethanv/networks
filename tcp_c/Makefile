all: client server

clean:
	rm -vf sort *.o client server

client: tcpclient.c
	gcc -g -o client tcpclient.c

server: tcpserver.c
	gcc -g -o server tcpserver.c

runc:
	./client 127.0.0.1 30003

runs:
	./server 30003
