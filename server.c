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

// int read_from_client (int filedes, char *usernames)
// {
//   char buffer[256];
//   int nbytes;
//   char name;

//   nbytes = read (filedes, buffer, 256);
//   if (nbytes < 0) {
//     error("ERROR in read");
//   }
//   else if (nbytes == 0)
//     return -1;
//   else
//   {
//     // name = usernames[filedes];
//     if (usernames[filedes] == NULL) {
//       fprintf (stderr, "%s joined the room", buffer);
//       strcpy(&usernames[0], buffer);
//     } else {
//       fprintf (stderr, "%s: %s", name, buffer);  
//     }
//     return 0;
//   }
// }

int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  char buffer[256];
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
  char usernames[10000][20];
  char name;



  FD_ZERO (&active_fd_set);
  FD_SET (sockfd, &active_fd_set);

  while (1) {
    read_fd_set = active_fd_set;
    if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
      error("ERROR in select");
    }

    for (i = 0; i < FD_SETSIZE; i++) {
      if (FD_ISSET(i, &read_fd_set)) {
        if (i == sockfd) { // new connection
          int new;
          size = sizeof (clientname);
          new = accept (sockfd,
                              (struct sockaddr *) &clientname,
                              &size);
          if (new < 0) {
            perror ("accept");
            exit (EXIT_FAILURE);
          }
          fprintf (stderr,
                   "Server: connect from host %i, port %hd.\n",
                   inet_ntoa (clientname.sin_addr),
                   ntohs (clientname.sin_port));
          FD_SET (new, &active_fd_set);

        } else { // new message from existing connection
          bzero(buffer, 256);
          if (read (i, buffer, 256) <= 0) {
            close (i);
            FD_CLR (i, &active_fd_set);
          } else {
            if (usernames[i] == NULL) {
              fprintf (stderr, "%s joined the room", buffer);
              strcpy(usernames[i], buffer);
            } else {
              fprintf (stderr, "new message %s", buffer);
              fprintf (stderr, "%s", usernames[i]);
            }
          }
        }
      }
    }

  }


  close(sockfd);
  return 0; 
}
