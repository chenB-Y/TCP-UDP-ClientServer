#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SIM_LENGTH 10 
#define IP_ADDRESS "127.0.0.1" 
#define PORT 1337 

int main(void)
{
    int sock; 					// Holds socket file descriptor
    struct sockaddr_in cli_name; 	// Holds Destination details (IP+PORT)
    int count;
    int value;
    int retVal;

    printf("Client is alive and establishing socket connection.\n");

    sock = socket(AF_INET, SOCK_STREAM, 0);	// Create socket
    if (sock < 0)								// Validate the socket
    {
        perror("Error opening channel");
        close(sock);
        exit(1);
    }


    bzero(&cli_name, sizeof(cli_name)); 				// Clearing the struct
    cli_name.sin_family = AF_INET; 					// Setting IPv4
    cli_name.sin_addr.s_addr = inet_addr(IP_ADDRESS); // Setting IP Address
    cli_name.sin_port = htons(PORT); 					// Setting Port Number

    // Connecting to server and Validating connection
    if (connect(sock, (struct sockaddr*)&cli_name, sizeof(cli_name)) < 0)
    {
        perror("Error establishing communications");
        close(sock);
        exit(1);
    }

    // Reads information from server
    for (count = 1; count <= SIM_LENGTH; count++)
    { //read(sock, &value, 4);
        retVal = recv(sock, &value, 4, 0);	// get data
        if (retVal > 0)
            printf("Client has received %d from socket.\n", value);
    }

    printf("Exiting now.\n");

    close(sock); 	// Closing Socket
    exit(0);

}