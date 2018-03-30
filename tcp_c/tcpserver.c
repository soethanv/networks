#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <stdlib.h>
#define Q_SIZE 5
#define BUFSIZE 1024

int main(int argc, char* argv[]){
    int status, addrlen, size, nbytes, flags, sock, new_sock, pid;
    int port; /* protocol port number */
    struct sockaddr_in server; /* receiver */
    struct sockaddr_in sender;  /* source - sender */

    if(argc < 2){
        printf("Usage: %s port-number\n", argv[0]);
        exit(1);
    }
    
    char *msg_buf; /* message buffer */
    msg_buf = (char *) malloc(BUFSIZE * sizeof(char *));
    if(msg_buf == NULL) {
        printf("Could not allocate memory!");
        exit(1);
    }
    port = atoi(argv[1]);   /* convert to binary */

    /* Create master socket */
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if( sock < 0 ){
        printf("socket error = %d\n", sock);
        return -1;
    }
    printf("Master Socket created\n");

    /* fill in address and port for server*/
    server.sin_family = AF_INET;    
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(port);

    /*associate the socket with the address structure - binding*/
    status = bind( sock, (struct sockaddr*) &server, sizeof(server));
    if( status < 0 ){
        printf("bind result: %d\n", status);
        return -1;
    }
    /*server listening: Q_SIZE defines number of connections accepted*/
    status = listen(sock, Q_SIZE);
    if( status < 0){
        printf("listen Error: %d\n", status);
        return -1;
    }
    printf("TCP server is ready! Watching at port: %d \n", port);
    
    addrlen = sizeof(sender);
    nbytes = sizeof(msg_buf);
    flags = 0;

    /* handle client loop */
    while( (new_sock = accept(sock, (struct sockaddr*)&sender, &addrlen)) > 0) {
        switch( pid = fork() ){
            case -1:
                perror("fork error");
                return -1;
            case 0: // child process
                close(sock);
                /*Read BUFSIZE*/
                while( (status = recv(new_sock, msg_buf, BUFSIZE, flags)) > 0 ){
                    printf("Message: %s", msg_buf);
                    char msg[17] = "Message Received";
                    
                    status  = send(new_sock, msg, sizeof(msg), flags);
                    if( status < 0 ){
                        perror("send() failed");
                        close(new_sock);
                        return -1;
                    }
                    memset(msg_buf,0 , BUFSIZE);
                }
                close(new_sock);
                break;
            default: // parent process
                close(new_sock);
        }
    }
    
    close(sock); /* close socket */
    return 0;
}
