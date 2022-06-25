/*

First version of the server (not concurrent)

*/

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define HELLO "Hello from server"

int main(int argc, char const* argv[])
{
    // The program can be executed with either 1 or 2 parameters (port)
    // By default is 8080
    unsigned short port = 8080;

    if (argc != 1) {
        if (argc != 2) {
            perror("The program needs either none or one argument (the port)");
            exit(EXIT_FAILURE);
        } else {
            // Set the given port
            port = (unsigned short) atoi(argv[1]);
        }
    }

    int socket_filedescriptor, new_socket, valread;

    // This is the data type used to represent socket addresses in the Internet namespace
    struct sockaddr_in address;

    int address_len = sizeof(address);
    int opt = 1;

    char buffer[1024] = { 0 };

    // Initializing address structure
    // This identifies the address family or format of the socket address
    address.sin_family = AF_INET;
    // You can use this constant to stand for “any incoming address” when binding to an address
    address.sin_addr.s_addr =  htonl (INADDR_ANY);
    // htons converts the uint16_t integer hostshort from host byte order to network byte order.
    address.sin_port = htons (port);
 
    // Creating socket file descriptor
    // socket() is a method. This function creates a socket and specifies communication style
    // PF_INET is used to specife the namespace (or domain) for IPv4 addresses
    // SOCK_STREAM is the style of communication (basically this is TCP)
    // The last int is the protocol (the standard is 0)
    if ((socket_filedescriptor = socket(PF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
 
    // Reuse of local addresses for this socket
    // setsockopt is a method that set the options for the socket
    if (setsockopt(socket_filedescriptor, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt error");
        exit(EXIT_FAILURE);
    }
 
    // Forcefully attaching socket to the port 8080
    // After the socket was created and set the options we can bind 
    // The bind function assigns an address to the socket socket
    // This function gets: a socket, a address and the lenght of the address
    if (bind(socket_filedescriptor, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // After the socket is all set, we can start waiting for connections from clients
    // The second int describe how many connections the server can accept (in this case is 8)
    if (listen(socket_filedescriptor, 8) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(socket_filedescriptor, (struct sockaddr *) &address, (socklen_t*) &address_len)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    if (recv(new_socket, buffer, sizeof(buffer), 0) == -1) {
        perror("Reading error");
        exit(EXIT_FAILURE);
    }

    // valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);

    if (send(new_socket, HELLO, strlen(HELLO), 0) < 0) {
        perror("Sending error");
        exit(EXIT_FAILURE);
    }
    // send(new_socket, HELLO, strlen(HELLO), 0);
    printf("Hello message sent\n");
   
    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    shutdown(socket_filedescriptor, SHUT_RDWR);
    return 0;
}