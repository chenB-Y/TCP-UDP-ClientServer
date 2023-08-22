
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
#define IP_ADDRESS "127.0.0.1"

int main(int argc, char* argv[])
{
	/*************************************** Vars ***************************************************/
	struct sockaddr_in  dest;	// Holds Destination socket (IP+PORT)
	int socket_fd;			// Holds socket file descriptor
	unsigned int ssize;		// Holds size of dest
	struct hostent* hostptr;	// Holds host information
	char buf[BUF_SIZE + 1];		// Used for writing/ Reading from buffer
	int retVal = 0;			// Holds return Value for recvfrom() / sendto()
	char* cRetVal = NULL;		// Holds return Value for fgets()




/************************************* Initialization ******************************************/


	socket_fd = socket(AF_INET, SOCK_DGRAM, 0);	// Create socket
	if (socket_fd == -1)
	{
		perror("Create socket"); exit(1);
	}	// Validate the socket

	if (fcntl(socket_fd, F_SETFL, O_NONBLOCK) != 0)
	{
		perror("Blocking"); exit(1);
	} 	// Set socket for non-blocking

	bzero((char*)&dest, sizeof(dest)); 		// Clearing the struct
	dest.sin_family = (short)AF_INET;		// Setting IPv4 
	dest.sin_port = htons((u_short)1337);		// Setting port 
	dest.sin_addr.s_addr = inet_addr(IP_ADDRESS); 	// Setting IP address

//	hostptr = gethostbyname(argv[1]);					// Getting host name
//	bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);	// Setting host name into struct

	ssize = sizeof(dest);							// Get dest size





/************************************* Code *****************************************************/

	while (1)
	{
		cRetVal = fgets(buf, BUF_SIZE, stdin);			// Get input from user
		if (cRetVal == NULL) { perror("Input error"); exit(1); }	// Make sure input OK

		retVal = sendto(socket_fd, buf, BUF_SIZE, 0, (struct sockaddr*)&dest, sizeof(dest));	// Send Ping
		if (retVal < 0) { perror("Write socket error"); exit(2); }					// Make sure it was sent

		retVal = recvfrom(socket_fd, buf, BUF_SIZE, 0, (struct sockaddr*)&dest, &ssize);		// Get answer
		while (retVal <= 0)	// Stop when answer rcved
		{
			sleep(1);	// Wait 1 Sec.
			retVal = recvfrom(socket_fd, buf, BUF_SIZE, 0, (struct sockaddr*)&dest, &ssize);	// Try getting answer again
		}

		printf("%s\n", buf);
	}

	close(socket_fd);	// Closing socket
	return 0;
