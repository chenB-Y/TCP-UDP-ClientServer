#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define BUF_SIZE 60

int main(int argc, char* argv[])
{
	/*********** Vars ***************/
	int socket_fd;				// Socket FD and answer number
	int retVal;				// Holds return value
	unsigned int fsize;			// Holds from size 
	struct sockaddr_in  s_in, from;		// Socket variables 
	char buf[BUF_SIZE + 1];			// Holds data from socket
	char ping[10] = "Ping\n";		// Ping message struct

/******* Initialization	********/
	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);	// Create socket

	bzero((char*)&s_in, sizeof(s_in));		// Clearing the struct
	s_in.sin_family = (short)AF_INET;		// Setting IPv4 
	s_in.sin_addr.s_addr = htonl(INADDR_ANY);	// Setting IP address
	s_in.sin_port = htons((u_short)1337);		// Setting port

	bind(socket_fd, (struct sockaddr*)&s_in, sizeof(s_in));	// Bind the socket to IP+PORT
	fsize = sizeof(from);

	if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) != 0)
	{
		perror("Blocking"); exit(1);
	} 	// Set socket for non-blocking

/*********** Code *************/
	while (1)
	{
		printf("."); fflush(stdout);
		sleep(1);
		retVal = recvfrom(socket_fd, buf, BUF_SIZE, 0, (struct sockaddr*)&from, &fsize);	/* get data */

		if (retVal > 0) {
			if (strcmp(buf, ping) == 0)  // check if "Ping\n"
			{ // We got Ping
				sendto(socket_fd, "Pong", BUF_SIZE, 0, (struct sockaddr*)&from, sizeof(from)); /* return pong to sender */
			}
			else
			{ // We didn't get Ping
				sendto(socket_fd, "What are we playing?", BUF_SIZE, 0, (struct sockaddr*)&from, sizeof(from)); /* return question to sender */
				printf("\nReceived packet from %s:%d\nData: %s\n",
					inet_ntoa(from.sin_addr), ntohs(from.sin_port), buf);
			}
		}
	}

	return 0;
}
