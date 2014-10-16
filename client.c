// client.c
//  Modified client code from http://www.linuxhowtos.org/C_C++/socket.htm
//  Alex Standke

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <pthread.h>
#include <stdlib.h>


void error(const char *msg)
{
    perror(msg);
    exit(0);
}

// Get new messages
void read_data(const int sockfd) {
    char messages[1000][255]; // TODO - clear message log occasionally to avoid segfault
    int messages_recorded = 0;

    while (1) {
        int count;
        char buffer[256];
        ioctl(sockfd, FIONREAD, &count);

        // If there's new data, read it
        if (count != 0) {
            bzero(buffer,256);
            read(sockfd, buffer, 255);
            strcpy(messages[messages_recorded], buffer);
            messages_recorded++;

            // Print list of messages again
            system("clear");
            int i;
            for(i = 0; i < messages_recorded; i++)
            {
               printf("%s", messages[i]);
            }
        }
    }
}

// Collect a new message
void write_data(const int sockfd) {
    while (1) {
        char buffer[256];
        struct timeval wait_time;
        
        printf("> ");
        bzero(buffer,256);
        fgets(buffer,255,stdin);
        if (strlen(buffer) > 1) { // only write message if it contains more than a newline
            write(sockfd,buffer,strlen(buffer));
            bzero(buffer,256);
        }
    }
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    struct timeval wait_time;

    char buffer[256];
    
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    server = gethostbyname(argv[1]);
    
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    printf("Please enter your name: ");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    
    pthread_t reader, writer;
    pthread_create( &reader, NULL, read_data, sockfd);
    pthread_create( &writer, NULL, write_data, sockfd);
    pthread_join( reader, NULL);
    pthread_join( writer, NULL);

    close(sockfd);
    return 0;
}
