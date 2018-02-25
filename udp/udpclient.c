/* SOE THAN */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]){

    int sock, status, nbytes, flags, size;
    int port; /* protocol port number */
    struct sockaddr_in destination; /* destination */
    struct sockaddr_in source;      /* source */

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
    char ip_addr[16]; /* destination IP address */

    /* user inputs for a message */
    printf("Message - max %d chars: ", BUFSIZE);
    fgets(msg_buf, BUFSIZE, stdin);
    strcpy(ip_addr, argv[1]);
    port = atoi(argv[2]);   /* convert to binary */

    /* create a socket to send on */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
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

    /* binding - associate the socket with the address structure*/
    status = bind( sock, (struct sockaddr*) &source, sizeof(source));
    if( status < 0 ){
        printf("bind result: %d\n", status);
        return -1;
    }

    /* the inet_addr function converts a
    string form of IP address to a 32 binary
    integer "in network order" */
    destination.sin_addr.s_addr = inet_addr(&ip_addr[0]);

    nbytes = strlen(msg_buf)+1;
    flags = 0;

    while(1){
        /* send a message to the server */
        size = sendto(sock, (char *) msg_buf, nbytes, flags,
            (struct sockaddr*)&destination, sizeof(destination));

        if( size < 0 ){
            perror("sendto failed");
            return -1;
        }
        /* NOTE: sendto success does NOT mean it is delivered.*/
        printf("sendto success\n");
        
        /* user inputs for another message */
        memset(msg_buf, 0, BUFSIZE);
        printf("Message - max %d chars: ", BUFSIZE);
        fgets(msg_buf, BUFSIZE, stdin);
        nbytes = strlen(msg_buf) +1;
    }

    free(msg_buf);
    close(sock);

    return 0;
}
