#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 8080

#define HELLO "Hello from client"
 
int main(int argc, char const* argv[]) {

    // The program can be executed with either 1 or 3 parameters (server address, port)
    // By default is 8080
    unsigned short port = 8080;

    struct sockaddr_in server_address;

    // Initializing socket structure with NULL
	memset(&server_address, '\0', sizeof(server_address));

    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        printf("Invalid address / Address not supported.\n");
        exit(EXIT_FAILURE);
    }


    // The program can be executed with either 1 or 3 parameters (Host and port)
    if (argc != 1) {
        if (argc != 3) {
            perror("The program needs either none or two arguments (server address, port).");
            exit(EXIT_FAILURE);
        } else {
            // Set the given address and port
            if (inet_pton(AF_INET, argv[1], &server_address.sin_addr) <= 0) {
                printf("Invalid address / Address not supported.\n");
                exit(EXIT_FAILURE);
            }
            port = (unsigned short) atoi(argv[2]);
        }
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons (port);

    int serverSocket, client_fd;

    char buffer[1024] = { 0 };

    if ((serverSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error.\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Socket has been created.\n");
    }
 
    if ((client_fd = connect(serverSocket, (struct sockaddr*) &server_address, sizeof(server_address))) < 0) {
        printf("Connection Failed.\n");
        exit(EXIT_FAILURE);
    } else {
        printf("The connection has been opened.\n");
    }
	
    if (send(serverSocket, HELLO, strlen(HELLO), 0) < 0) {
        perror("Sending error.\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Hello message sent.\n");
    }

    if (recv(serverSocket, buffer, sizeof(buffer), 0) < 0) {
        perror("Reading error.\n");
        exit(EXIT_FAILURE);
    } else {
        printf("%s\n", buffer);
    }
    
    // closing the connected socket
    close(client_fd);
    close(serverSocket);
    printf("Client Ended Successfully.\n");
    return 0;
}
