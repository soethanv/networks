/* SOE THAN */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 1024

int main(int argc, char* argv[]){
    int status, addrlen, size, nbytes, flags, sock;
    char *msg_buf; /* message buffer */
    int port; /* protocol port number */

    struct sockaddr_in server; /* receiver */
    struct sockaddr_in sender;  /* source - sender */

    if(argc < 2){
        printf("Usage: %s port-number", argv[0]);
    }
    
    msg_buf = (char *) malloc(BUFSIZE * sizeof(char *));
    if(msg_buf == NULL) {
        printf("Could not allocate memory!");
        exit(1);
    }
    port = atoi(argv[1]);   /* convert to binary */

    /* create a socket to receive message on */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if( sock < 0 ){
        printf("socket error = %d\n", sock);
        return -1;
    }

    /* fill in address and port for server*/
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    /*notice this struct within a struct */

    server.sin_port = htons(port);

    /*associate the socket with the address structure - this is called binding*/
    status = bind( sock, (struct sockaddr*) &server, sizeof(server));
    if( status < 0 ){
        printf("bind result: %d\n", status);
        return -1;
    }else
        printf("UDP server is ready! Listening at PORT: %d \n", port);

    /*
    The recvfrom function is a read and the arguments are:
        sock - the socket we are reading
        buffer - array into which to read the data
        nbytes - read up to this many bytes
        flags - used for special purposes - not needed here
        from - sockaddr struct to hold the IP address and port of the sender of the packet
        addrlen - the size of teh sockaddr struct written by this function
    */
    addrlen = sizeof(sender);
    nbytes = sizeof(msg_buf);
    flags = 0;

    while(1){
        /* receive message from client */
       size = recvfrom(sock, msg_buf, BUFSIZE, flags, (struct sockaddr*)&sender, &addrlen);
        if( size < 0 ) {
            printf("recvfrom() error\n");
            return -1;
        }
        printf("Message received: %s",msg_buf);
        printf("Sender information:\n\tIP: %s\n\tPort: %d\n", inet_ntoa(sender.sin_addr), ntohs(sender.sin_port));
        printf("\nwaiting on port: %d\n", port);
        memset(msg_buf, 0, BUFSIZE);
    }

    free(msg_buf);
    close(sock); /* close socket */
    return 0;
}
