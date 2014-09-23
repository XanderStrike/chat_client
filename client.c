//**************************************
//INCLUDE files for :Send a TCP packet to a server
//**************************************
/* The Includes */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

//**************************************
// Name: Send a TCP packet to a server
// Description:/* The purpose of this article is to help out people who know the basics of C but want to start learning TCP controls in C. This program will connect to a server and send a TCP packet containing "La la la la". */
// By: Markus Delves
//
//
// Inputs:usage: program_name <ip address> <port>
//
// Returns:The program will tell you if it was successful or not
//
//Assumes:None
//
//Side Effects:None known
//This code is copyrighted and has limited warranties.
//Please see http://www.Planet-Source-Code.com/xq/ASP/txtCodeId.717/lngWId.3/qx/vb/scripts/ShowCode.htm
//for details.
//**************************************

int sock;
// Start main with command line arguments


    int main(int argc, char *argv[]) {
    // Get ready for the TCP stuff
    struct hostent *he; // Used for DNS lookup
    struct sockaddr_in blah; // inet addr stuff
    // Create a varible for our packet
    // Remember, TCP packets max at 1024
    char packet[1024];
    // A varible to hold the servers' address
    char *address;
    // A varible for the port
    int port;
    // Extra vars
    int i;
        // Make sure two arguments were supplied


            if (argc != 3) {
            // Print how-to use the program then exit
            fprintf(stderr, "usage: %s <ip address> <port>\n",argv[0]);
            return(-1);
            } 
            // We know there are two arguments
            // so let's use them.
            address = argv[1];
            port = atoi(argv[2]);
            // Create the unconnected socket
            sock = socket (AF_INET, SOCK_STREAM, 0);
            //Set some settings
            blah.sin_family = AF_INET; //we're using inet
            blah.sin_port = htons (port); //set the port
            he = gethostbyname (address); //set the address
            fprintf(stderr, "Attempting a connection with %s on port %d\n", address, port);
            // Is the ip/hostname working?
            if (!he) 


                {
                if ((blah.sin_addr.s_addr = inet.addr (address)) == ADDR_NONE)
                return(-1);


                    } else {
                    bcopy (he->h_addr, (struct in_addr *) &blah.sin_addr, he->h_length);
            }
            // Did they accept us?
            if (connect (sock, (struct sockaddr *) &blah, sizeof
            (blah)) < 0)


                {
                fprintf(stderr, "Connection refused by remote host.\n");
                return(-1);
            }
            //Create the packet
            sprintf(packet, "La la la la");
            //And send it
            write (sock, packet, strlen(packet));
            close (sock); // Close the connection
            fprintf(stderr, "Operation Completed. Exiting...");
        }

    
