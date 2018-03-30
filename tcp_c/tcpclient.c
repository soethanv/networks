#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]){
    
    int sock, status, nbytes, flags, size;
    int port; /* protocol port number */
    struct sockaddr_in destination; /* destination */
    struct sockaddr_in source;      /* source */
    
    char ip_addr[16]; /* destination IP address */

    if (argc < 3) {
      printf("Usage: %s server-ip port-number\n",argv[0]);
      exit(-1);
    }

    char *msg_buf; /* message buffer */
    msg_buf = (char *) malloc( BUFSIZE * sizeof(char *));
    if(msg_buf == NULL) {
        printf("Could not allocate memory!");
        exit(1);
    }
    /* user inputs for a message and an IP address */
    printf("Message - max %d chars: ", BUFSIZE);
    fgets(msg_buf, sizeof(msg_buf), stdin);

    strncpy(ip_addr, argv[1], sizeof(ip_addr));
    port = atoi(argv[2]);   /* convert to binary */

    /* create a socket to send on */
    /* Notice SOCK_STREAM here */
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if( sock < 0 ){
        printf("socket error = %d\n", sock);
        return -1;
    }

    /* fill in the address family and port for destination */
    destination.sin_family = AF_INET;
    destination.sin_port = htons(port);

    /* fill in address and port for source */
    source.sin_family = PF_INET;
    source.sin_port = htons(0);
    source.sin_addr.s_addr = htonl(INADDR_ANY);

    /* binding - associate the socket with the address structure f*/
    status = bind( sock, (struct sockaddr*) &source, sizeof(source));
    if( status < 0 ){
        printf("bind result: %d\n", status);
        return -1;
    }
    
    /* the inet_addr function converts 
        a string form of IP address to a 32 binary
        integer "in network order" 
    */
    destination.sin_addr.s_addr = inet_addr(&ip_addr[0]);


    /*establish connection with the server*/
    status = connect( sock, (struct sockaddr*) &destination, sizeof(destination) );
    if( status < 0){
        printf("connect result: %d\n", status);
        return -1;
    }
    //printf("Connecion accepted with server %s...\n",
    //    inet_ntop(AF_INET, &(destination.sin_addr), ip_addr, sizeof(ip_addr)));

    nbytes = strlen(msg_buf)+1;
    flags = 0;

    while(1){
        /* send a message to the server */
        printf("sending msg_buf = %s\n", msg_buf);
        status = send(sock, (char *) msg_buf, nbytes, flags);
        if( status < 0 ){
            perror("send failed");
            return -1;
        }

        /*receive reply message from server */
        char msg[20];
        status = recv(sock, (char*) msg, sizeof(msg), flags);
        if( status < 0 ){
            perror("recv failed");
            return -1;
        }
        printf("%s\n", msg);

        /* user inputs for another message */
        memset(msg_buf, 0, sizeof(msg_buf)); //flashout the buffer
        printf("Message - max %d chars: ", BUFSIZE);
        fgets(msg_buf, BUFSIZE, stdin);
        nbytes = strlen(msg_buf) +1;
    }
    free(msg_buf);
    close(sock);
    return 0;
}
