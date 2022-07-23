#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

/*
Server application for CTris Project
*/

// Function that end successfully the program sending the given result
void endSuccessProgramAfterCheckresult(char board [], int newClientSocket, int socket_filedescriptor) {

    // Send the last board (where the first char has the result)
    if (write(newClientSocket, board, strlen(board)) < 0) {
        perror("Sending error");
        exit(EXIT_FAILURE);
    } else {
        printf("Last board sent.\n");
    }

    close(newClientSocket);
    close(socket_filedescriptor);
    printf("\nServer Ended Successfully.\n\n\n");
    exit(EXIT_SUCCESS);

}


// Auxiliary function to copy the data inside the board into the buffer
void copyBoardToBuffer(char board[], char buffer[]) {
    for(int i = 0; i < 10; i++) {
        buffer[i] = board[i];
    }
}


// Function that returns true if the game has ended (That is when there are no more free spaces).
bool checkEndGame(char board []) {
    for(int i = 1; i < 10; i++) {
        if (board[i] == i+'0') return false;
    }
    return true;
}

// Auxiliary function that ends the server and prints the given message.
void endServerError(const char * errMessage, int newClientSocket, int socket_filedescriptor) {
    printf("\nError Message: %s", errMessage);
    close(newClientSocket);
    close(socket_filedescriptor);
    printf("\nServer Ended.\n");
    exit(EXIT_FAILURE);
}

// This function check if the game has been won by the computer or the player (or if it's a draw).
void checkResult(char board [], int newClientSocket, int socket_filedescriptor) {

    if (board[1] == board[2] && board[2] == board[3]) {
        if (board[1] == 'O') {
            board[0] = 'S';
        } else if (board[1] == 'X') {
            board[0] = 'V';
        } else {
            endServerError("Error during check result.\n", newClientSocket, socket_filedescriptor);
        }
        
        endSuccessProgramAfterCheckresult(board, newClientSocket, socket_filedescriptor);
    }
        
    else if (board[4] == board[5] && board[5] == board[6]) {
        if (board[4] == 'O') {
            board[0] = 'S';
        } else if (board[4] == 'X') {
            board[0] = 'V';
        } else {
            endServerError("Error during check result.\n", newClientSocket, socket_filedescriptor);
        }
        
        endSuccessProgramAfterCheckresult(board, newClientSocket, socket_filedescriptor);
    }
        
    else if (board[7] == board[8] && board[8] == board[9]) {
        if (board[7] == 'O') {
            board[0] = 'S';
        } else if (board[7] == 'X') {
            board[0] = 'V';
        } else {
            endServerError("Error during check result.\n", newClientSocket, socket_filedescriptor);
        }
        
        endSuccessProgramAfterCheckresult(board, newClientSocket, socket_filedescriptor);
    }
        
    else if (board[1] == board[4] && board[4] == board[7]) {
        if (board[1] == 'O') {
            board[0] = 'S';
        } else if (board[1] == 'X') {
            board[0] = 'V';
        } else {
            endServerError("Error during check result.\n", newClientSocket, socket_filedescriptor);
        }
        
        endSuccessProgramAfterCheckresult(board, newClientSocket, socket_filedescriptor);
    }
        
    else if (board[2] == board[5] && board[5] == board[8]) {
        if (board[2] == 'O') {
            board[0] = 'S';
        } else if (board[2] == 'X') {
            board[0] = 'V';
        } else {
            endServerError("Error during check result.\n", newClientSocket, socket_filedescriptor);
        }
        
        endSuccessProgramAfterCheckresult(board, newClientSocket, socket_filedescriptor);
    }
        
    else if (board[3] == board[6] && board[6] == board[9]) {
        if (board[3] == 'O') {
            board[0] = 'S';
        } else if (board[3] == 'X') {
            board[0] = 'V';
        } else {
            endServerError("Error during check result.\n", newClientSocket, socket_filedescriptor);
        }
        
        endSuccessProgramAfterCheckresult(board, newClientSocket, socket_filedescriptor);
    }
        
    else if (board[1] == board[5] && board[5] == board[9]) {
        if (board[1] == 'O') {
            board[0] = 'S';
        } else if (board[1] == 'X') {
            board[0] = 'V';
        } else {
            endServerError("Error during check result.\n", newClientSocket, socket_filedescriptor);
        }
        
        endSuccessProgramAfterCheckresult(board, newClientSocket, socket_filedescriptor);
    }
        
    else if (board[3] == board[5] && board[5] == board[7]) {
        if (board[3] == 'O') {
            board[0] = 'S';
        } else if (board[3] == 'X') {
            board[0] = 'V';
        } else {
            endServerError("Error during check result.\n", newClientSocket, socket_filedescriptor);
        }
        
        endSuccessProgramAfterCheckresult(board, newClientSocket, socket_filedescriptor);

    } else {

        // If the game has not ended it will just return
        if (!checkEndGame(board)) {
            return;
        }

        // Check if the game has ended with a draw.

        bool flagForDraw = true;

        for (int i = 1; i < 10; i++) {
            if (board[i] != 'X' && board[i] != 'O') flagForDraw = false;
        }

        if (flagForDraw) {
            board[0] = 'P';
            endSuccessProgramAfterCheckresult(board, newClientSocket, socket_filedescriptor);
        } else {
            endServerError("Error during check result.\n", newClientSocket, socket_filedescriptor);
        }
        
    }

}

// Function used to return a "legal" random move from the computer
void moveCPURandom(char board []) {

    // Use current time as seed for random generator
    srand(time(0));

    int randomChoice = rand() % 9;

    randomChoice++;

    while (true) {
        if(board[randomChoice] == randomChoice+'0') {
            board[randomChoice] = 'O';
            return;
        } else {
            randomChoice = rand() % 9;
            randomChoice++;
        }
    }
}

int main(int argc, char const* argv[]) {

    // The program can be executed with either none or 1 parameter (port)
    // By default is 8080
    unsigned short port = 8080;

    // Check if it's also given the port
    if (argc != 1) {
        if (argc != 2) {
            perror("The program needs either none or one argument (the port)");
            exit(EXIT_FAILURE);
        } else {
            // Set the given port
            port = (unsigned short) atoi(argv[1]);
        }
    }

    // These are the 2 int that represents the socket and the new connection opened with the client
    int socket_filedescriptor, newClientSocket;

    // This is the data type used to represent socket addresses in the Internet namespace
    struct sockaddr_in address;

    int address_len = sizeof(address);

    // Used when the fork() system call is executed
    pid_t pid = 0;

    // Hello message to send to the client
    char * hello = "Hello from server";
    // This is the variable that will be used for every communication (forward and backward) with the client
    char buffer[100] = { 0 };

    // Initializing address structure with NULL
	memset(&address, '\0', sizeof(address));

    // Initializing address structure
    // This identifies the address family or format of the socket address
    address.sin_family = AF_INET;
    // You can use this constant to stand for “any incoming address” when binding to an address
    address.sin_addr.s_addr =  htonl (INADDR_ANY);
    // htons converts the uint16_t integer hostshort from host byte order to network byte order.
    address.sin_port = htons (port);
 
    // Creating socket file descriptor
    // socket() is a system call. This function creates a socket and specifies communication style.
    // PF_INET is used to specify the namespace (or domain) for IPv4 addresses.
    // SOCK_STREAM is the style of communication (basically this is TCP)
    // The last int is the protocol (the standard is 0)
    if ((socket_filedescriptor = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Server Socket has been created.\n");
    }
 
    // Forcefully attaching socket to the port 8080 (or the given one).
    // The bind function assigns an address to the socket that will be used.
    // This function gets: a socket, a address and the lenght of the address.
    if (bind(socket_filedescriptor, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    } else {
        printf("Bind instruction was successfull.\n");
    }

    // After the socket is all set, we can start waiting for connections from clients.
    // The second int describe how many connections the server can accept (in this case is 8).
    if (listen(socket_filedescriptor, 8) < 0) {
        perror("Listen");
        exit(EXIT_FAILURE);
    } else {
        printf("Listening...\n\n");
    }

    // Number of clients connected
    int nClient = 0;

    while (true) {

        // With the function accept we can initialize a new connection with the client that has requested a comunication.
        if ((newClientSocket = accept(socket_filedescriptor, (struct sockaddr *) &address, (socklen_t*) &address_len)) < 0) {
            perror("Accept Error");
            exit(EXIT_FAILURE);
        }

        // Prints the information of the client connected.
        printf("Connection accepted from %s:%d\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        nClient++;
        printf("Clients connected: %d\n", nClient);

        // With the fork() system call here is generated a new process that will manage the game itself with client.
        // (The parent process will just go on top waiting for a new incoming connection.)
        if ((pid = fork()) < 0) {

            perror("Fork Error");
            exit(EXIT_FAILURE);

        } else if(pid == 0) {

            // Close the socket for this child process.
            close(socket_filedescriptor);

            // From here start the game itself

            // The instruction bzero() clears the associated memory passed as the argument.
            bzero(buffer, sizeof(buffer));
            
            // Read the hello message from the client.
            if (read(newClientSocket, buffer, sizeof(buffer)) < 0) {
                perror("Reading error.\n");
                exit(EXIT_FAILURE);
            } else {
                printf("%s\n", buffer);
            }

            // Sends the hello message to the client.
            if (write(newClientSocket, hello, strlen(hello)) < 0) {
                perror("Sending error");
                exit(EXIT_FAILURE);
            } else {
                printf("Hello message sent.\n");
            }

            // Read the message for starting the game.
            if (read(newClientSocket, buffer, sizeof(buffer)) < 0) {
                perror("Reading error.\n");
                exit(EXIT_FAILURE);
            } else {
                printf("%s\n", buffer);
            }

            bzero(buffer, sizeof(buffer));

            // This is the variable that will be used to store the board.
            // The first char is always used as a "state":
            // (V: Vittoria, S: Sconfitta, P: pareggio, O: is used to say that it's still going)
            char board [10] = {'O', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

            // Copy the board into the buffer.
            copyBoardToBuffer(board, buffer);

            // Sends the board.
            if (write(newClientSocket, buffer, strlen(buffer)) < 0) {
                perror("Sending error");
                exit(EXIT_FAILURE);
            } else {
                printf("Board sent.\n");
            }

            while (true) {

                bzero(buffer, sizeof(buffer));
            
                // First move from the player
                if (read(newClientSocket, buffer, sizeof(buffer)) < 0) {
                    perror("Reading error.\n");
                    exit(EXIT_FAILURE);
                } else {
                    printf("\nMossa scelta: %s\n", buffer);
                }

                // Sets the passed choice with X in the board.
                board[buffer[0]-'0'] = 'X';

                // Check if the game has ended.
                checkResult(board, newClientSocket, socket_filedescriptor);

                // Compute a new random move from the computer.
                moveCPURandom(board);

                // Check if the game has ended.
                checkResult(board, newClientSocket, socket_filedescriptor);

                bzero(buffer, sizeof(buffer));

                copyBoardToBuffer(board, buffer);

                // Sends the updated board (with the new moves from the client and the server)
                if (write(newClientSocket, buffer, strlen(buffer)) < 0) {
                    perror("Sending error");
                    exit(EXIT_FAILURE);
                } else {
                    printf("Board sent.\n");
                }
            }
            
            // The child process shouldn't arrive here, but just in case it closes the connection.
            close(newClientSocket);
            exit(EXIT_SUCCESS);
        }
    }

    close(newClientSocket);
    close(socket_filedescriptor);
    return 0;
}