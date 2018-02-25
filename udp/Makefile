all: client server

clean:
	rm -vf sort *.o

client: udpclient.c
	gcc -g -o client udpclient.c

server: udpserver.c
	gcc -g -o server udpserver.c

runc:
	./client 127.0.0.1 30003

runs:
	./server 30003
