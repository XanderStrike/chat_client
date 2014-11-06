/* A simple server in the internet domain using TCP
The port number is passed as an argument */

// http://www.gnu.org/software/libc/manual/html_node/Server-Example.html
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
  perror(msg);
  exit(1);
}

int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  char buffer[256];
  char message[512];
  struct sockaddr_in serv_addr, cli_addr;
  int n;

  int connections;
  int connfds[128];

  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) 
    error("ERROR opening socket");
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr,
    sizeof(serv_addr)) < 0) 
    error("ERROR on binding");


  listen(sockfd,5);

  fd_set active_fd_set, read_fd_set;
  int i;
  struct sockaddr_in clientname;
  socklen_t size;
  int nbytes;
  char usernames[1000][20];
  char name;

  for (i = 0; i < 1000; i++) {
    strcpy(usernames[i], "thisisnotsetwow");
  }


  FD_ZERO (&active_fd_set);
  FD_SET (sockfd, &active_fd_set);

  while (1) {
    read_fd_set = active_fd_set;
    if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
      error("ERROR in select");
    }

    for (i = 0; i < FD_SETSIZE; i++) {
      if (FD_ISSET(i, &read_fd_set)) {
        
        // new connection
        if (i == sockfd) {
          int new;
          size = sizeof (clientname);
          new = accept (sockfd, (struct sockaddr *) &clientname, &size);
          if (new < 0) {
            error("ERROR in accept");
          }
          fprintf (stderr,
                   "Server: connect from host %i, port %hd.\n",
                   inet_ntoa (clientname.sin_addr),
                   ntohs (clientname.sin_port));
          FD_SET (new, &active_fd_set);

        } 

        // New message from existing connection
        else {
          bzero(buffer, 256);
          bzero(message, 512);

          // EOF or whatever, close the socket
          if (read (i, buffer, 256) <= 0) {
            strcpy(message, usernames[i]);
            strcat(message, " left the room\n");
            close (i);
            strcpy(usernames[i], "thisisnotsetwow");
            FD_CLR (i, &active_fd_set);
          } 

          // We got some data!
          else {
            // It's a new user
            if (strcmp(usernames[i], "thisisnotsetwow") == 0) {
              strcpy(usernames[i], strtok(buffer, "\n"));
              strcpy(message, usernames[i]);
              strcat(message, " joined the room\n");
            }

            // Just a new message
            else {
              strcpy(message, usernames[i]);
              strcat(message, ": ");
              strcat(message, buffer);
            }
          }

          // Now we send 'message' to all clients, and print it out so the server is useful
          fprintf(stderr, "%s", message);

          // int n;
          // for (n = 0; n < FD_SETSIZE; n++) {
          //   if (FD_ISSET(n, &active_fd_set)) {
              write(i,message,512);
            // }
          // }
        }
      }
    }
  }


  close(sockfd);
  return 0; 
}
