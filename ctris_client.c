#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

/*
Client application for CTris Project
*/

// Auxiliary function to copy the data inside the buffer into the board
void copyBufferToBoard(char buffer[], char board[]) {
    for(int i = 0; i < 10; i++) {
        board[i] = buffer[i];
    }
}

// This function just check that the board contains the right symbols
void checkboard(char board []) {
    for (int i = 1; i < 10; i++) {
        
        if (board[i] != i+'0' && board[i] != 'X' && board[i] != 'O') {
            printf("Error: board received is incorrect.\n");
            exit(EXIT_FAILURE);
        }

    }
}

// This function is used to print the board
void showgameboard(char board []) {

    checkboard(board);

    // To simplify the player always use the symbol X
    printf("\nPlayer (X)  -  CPU (O)\n\n\n");

    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", board[1], board[2], board[3]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", board[4], board[5], board[6]);
    printf("_____|_____|_____\n");
    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", board[7], board[8], board[9]);
    printf("     |     |     \n\n");
}

// Function that asks the player to enter a position, then returns it
// (It keeps asking until the move is legal)
char move(char board []) {
    int moveChosen;

    bool legalMove = false;
    
    while (!legalMove) {
        printf("\nFai la tua mossa (scegli una tra le posizioni disponibili): ");
        scanf("%d", &moveChosen);

        if (board[moveChosen] != moveChosen+'0') {
            printf("\nLa posizione è già occupata");
        } else {
            legalMove = true;
        }
    }

    return moveChosen+'0';
}

// Auxiliary function that ends the program successfully.
void endSuccessProgram(int serverSocket) {
    close(serverSocket);
    printf("Client Ended Successfully.\n");
    exit(EXIT_SUCCESS);
}
 
int main(int argc, char const* argv[]) {

    // String used as a opening message
    char * hello = "Hello from client.";

    // This buffer will be used for the communication.
    char buffer[100] = { 0 };

    // The array board will be used to store the board of the game.
    // The first char is always used for the "state" of the game.
    char board[10] = { 0 };
    bzero(board, sizeof(board));

    // The program can be executed with either 1 or 3 parameters (server address, port)
    // By default the port is 8080
    unsigned short port = 8080;

    // Data structure used to store the address of the server
    struct sockaddr_in server_address;

    // Initializing socket structure with NULL
	memset(&server_address, '\0', sizeof(server_address));

    // By default the address is localhost (but it can be changed with args)
    if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
        printf("Invalid address / Address not supported.\n");
        exit(EXIT_FAILURE);
    }

    // The program can be executed with either none or 2 parameters (Host and port)
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

    // Set the address data structure.
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons (port);

    // int used for the socket.
    int serverSocket;

    // Creates the socket.
    if ((serverSocket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error.\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Socket has been created.\n");
    }

    // Open the connection to the given server's address
    if ((connect(serverSocket, (struct sockaddr*) &server_address, sizeof(server_address))) < 0) {
        printf("Connection Failed.\n");
        exit(EXIT_FAILURE);
    } else {
        printf("The connection has been opened.\n");
    }

    // First sends an hello message
    if (write(serverSocket, hello, strlen(hello)) < 0) {
        perror("Sending error.\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Hello message sent.\n");
    }

    // Read the message "Hello from message" and prints it
    if (read(serverSocket, buffer, sizeof(buffer)) < 0) {
        perror("Reading error.\n");
        exit(EXIT_FAILURE);
    } else {
        printf("%s\n", buffer);
    }

    // This function set to all 0 the memory area given as argument
    bzero(buffer, sizeof(buffer));

    // Change the message to "Starting the game..." and sends it
    hello = "Starting the game...";
    if (write(serverSocket, hello, strlen(hello)) < 0) {
        perror("Sending error.\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Starting game message sent.\n");
    }

    // From here starts the game.
    printf("\n\n\tTic Tac Toe\n\n");
	
    while (true) {

        // Clears the area associated to the buffer.
        bzero(buffer, sizeof(buffer));

        // Here the client should receive the updated board
        if (read(serverSocket, buffer, sizeof(buffer)) < 0) {
            perror("Reading error.\n");
            exit(EXIT_FAILURE);
        } else {

            // Copy the data received in the buffer into the board.
            copyBufferToBoard(buffer, board);

            // Here it will check if the game has ended, because each message regarding
            // the board has as the first character the outcome 
            // (V: Vittoria, S: Sconfitta, P: pareggio, O: is used to say that it's still going)
            if (board[0] == 'V') {
                showgameboard(board);
                printf("\nCongratulazioni, hai vinto la partita!!\n\n");
                endSuccessProgram(serverSocket);
            } else if (board[0] == 'S') {
                showgameboard(board);
                printf("\nPurtroppo hai perso la partita.\n\n");
                endSuccessProgram(serverSocket);
            } else if (board[0] == 'P') {
                showgameboard(board);
                printf("\nLa partita è finita in un pareggio.\n\n");
                endSuccessProgram(serverSocket);
            } else if (board[0] == 'O') {
                showgameboard(board);
            } else {
                // Case where there was an error
                printf("\nThere was an error with the result.\n");
                close(serverSocket);
                exit(EXIT_FAILURE);
            }
        }

        // Asks the next move from the player and sends it
        char moveChosen = move(board);
        if (write(serverSocket, &moveChosen, 1) < 0) {
            perror("Sending error.\n");
            exit(EXIT_FAILURE);
        }

    }

    // This also end the program successfully just in case a process arrives here
    close(serverSocket);
    printf("Client Ended Successfully.\n");
    return 0;
}