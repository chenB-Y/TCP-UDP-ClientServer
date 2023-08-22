#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>     
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#define SIM_LENGTH 10 
#define PORT 1337 

void clean_up(int cond, int* sock)
{
    printf("Exiting now.\n");
    close(*sock);
    exit(cond);
}

int main(void)
{
    int sock; 					// Holds socket file descriptor
    int connect_sock; 			// Holds new socket file descriptor
    struct sockaddr_in serv_name; // Holds Local details (IP+PORT)
    size_t len;
    int count;
    int retVal;

    sock = socket(AF_INET, SOCK_STREAM, 0);	// Create socket
    if (sock < 0)								// Validate the socket
    {
        perror("Error opening channel");
        clean_up(1, &sock);
    }

    bzero(&serv_name, sizeof(serv_name)); 		// Clearing the struct
    serv_name.sin_family = AF_INET;				// Setting IPv4
    serv_name.sin_port = htons(PORT); 			// Setting Port Number
    serv_name.sin_addr.s_addr = htonl(INADDR_ANY);// Setting IP Address

    // Binding the socket with the specific IP+PORT
    if (bind(sock, (struct sockaddr*)&serv_name, sizeof(serv_name)) < 0)
    {
        perror("Error naming channel");
        clean_up(1, &sock);
    }

    printf("Server is alive and waiting for socket connection from client.\n");
    listen(sock, 1); 	 // Listening for Connection Request

    len = sizeof(serv_name);
    // Accepting a connection request
    connect_sock = accept(sock, (struct sockaddr*)&serv_name, &len);

    // Sends information
    for (count = 1; count <= SIM_LENGTH; count++)
    {
        retVal = send(connect_sock, &count, 4, 0);	// Send message
        if (retVal < 0) { perror("Write socket error"); exit(2); }

        printf("Server has written %d to socket.\n", count);
    }

    // Closing Sockets
    close(connect_sock);
    clean_up(0, &sock);

}