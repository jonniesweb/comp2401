/*
 * main.c
 *
 *  Created on: Nov 28, 2013
 *      Author: Jonathan Simpson
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>

// define constants
#define SERVER_PORT 60002
#define BUFFER_SIZE 120
#define MAX_GUESSES 20

// result types
typedef enum {
	NONE, YES, NO, WIN
} result;

// gamemode types
typedef enum {
	INVALID, ORACLE, GUESSER
} gamemode;

// endgame types
typedef enum {
	EXIT, CONTINUE
} endgame;

// global variable for keeping track of which signal was run
// https://culearn.carleton.ca/moodle/mod/forum/discuss.php?d=64507
volatile result action = NONE;

/**
 * Sends whatever is in the buffer to whatever is at the other end of connection
 * @param connection The remote computer
 * @param buffer The content to send
 */
void sendMessage(int connection, char *buffer) {
	send(connection, buffer, strlen(buffer), 0);
}

/* BEGIN SIGNAL HANDLER HELPERS
 *
 * Keeps track of which signal was signalled
 */
void handleYes(int i) {
	action = YES;
}

void handleNo(int i) {
	action = NO;
}

void handleWin(int i) {
	action = WIN;
}
/* END SIGNAL HANDLER HELPERS */

/**
 * Waits for the remote machine to send a guess
 * @param connection The remote machine
 * @param guesses Number of guesses so far
 */
void receiveGuess(int connection, int guesses) {
	int bytesRcv;
	char buffer[BUFFER_SIZE];

	bytesRcv = recv(connection, buffer, sizeof(buffer), 0);
	buffer[bytesRcv] = 0;
	printf("Question %d:  %s\n", guesses, buffer);
}

/**
 * Waits for the remote machine to send a message
 * @param buffer Buffer to be written into
 * @param connection Socket of remote machine
 */
void getMessage(char buffer[BUFFER_SIZE], int connection) {
	int bytesRcv = recv(connection, buffer, sizeof(buffer), 0);
	buffer[bytesRcv] = 0;
}

/**
 * 'nuff said
 */
void printDatArt() {

	// escaped to the max
	puts(" ___   ___     ____                  _   _                 ");
	puts("|__ \\ / _ \\   / __ \\                | | (_)                ");
	puts("   ) | | | | | |  | |_   _  ___  ___| |_ _  ___  _ __  ___ ");
	puts("  / /| | | | | |  | | | | |/ _ \\/ __| __| |/ _ \\| '_ \\/ __|");
	puts(" / /_| |_| | | |__| | |_| |  __/\\__ \\ |_| | (_) | | | \\__ \\");
	puts("|____|\\___/   \\___\\_\\\\__,_|\\___||___/\\__|_|\\___/|_| |_|___/");
	puts("");

}

/**
 * Used by Guesser. Parses result of guess received from remote host.
 * @param connection Socket of remote machine
 * @return Returns YES if oracle says yes, NO if no, WIN for win, and NONE for error
 */
result receiveResult(int connection) {
	char buffer[BUFFER_SIZE];

	getMessage(buffer, connection);

	if (strcmp(buffer, "Yes") == 0) { // yes condition
		puts("Yes");
		return YES;
	} else if (strcmp(buffer, "No") == 0) { // no condition
		puts("No");
		return NO;
	} else if (strcmp(buffer, "Win!") == 0) { // win condition
		puts("Win!");
		return WIN;
	} else {
		fputs("Received invalid response from Oracle", stderr);
		return NONE;
	}
}

/**
 * Waits for the user to enter Ctrl-C, Ctrl-Z or Ctrl-\
 * @param connection The remote machine to send the decision to
 * @return Either returns WIN for win, or NONE for everything else
 */
result checkForSignal(int clientSocket) {
	puts("Enter Ctrl-C for Yes, Ctrl-Z for No, or Ctrl-\\ for Win");
	while (action == NONE) {
	}

	switch (action) {
	case YES:
		sendMessage(clientSocket, "Yes");
		puts("Yes");
		action = NONE;
		return NONE;
		break;
	case NO:
		sendMessage(clientSocket, "No");
		puts("No");
		action = NONE;
		return NONE;
		break;
	case WIN:
		sendMessage(clientSocket, "Win!");
		puts("Win!");
		action = NONE;
		return WIN;
		break;
	default:
		action = NONE;
		return NONE;
		break;
	}
	// TODO add a return
}

/**
 * Enables all required signals for the Oracle
 */
void enableSignals() {
	signal(SIGINT, handleYes);
	signal(SIGTSTP, handleNo);
	signal(SIGQUIT, handleWin);
}

/**
 * Disables all required signals for the Oracle
 */
void disableSignals() {
	signal(SIGINT, NULL);
	signal(SIGTSTP, NULL);
	signal(SIGQUIT, NULL);
}

/**
 * Open and accept incoming TCP connections
 * @param serverPort The port to listen on
 * @return The connection to the remote machine
 */
int socketServer(int serverPort) {

	int myListenSocket, clientSocket;
	struct sockaddr_in myAddr, clientAddr;
	int i, addrSize;

	/* create socket */

	myListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (myListenSocket < 0) {
		printf("Couldn't open socket\n");
		exit(-1);
	}

	/* setup my server address */
	memset(&myAddr, 0, sizeof(myAddr));
	myAddr.sin_family = AF_INET;
	myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myAddr.sin_port = htons((unsigned short) serverPort);

	/* bind my listen socket */

	i = bind(myListenSocket, (struct sockaddr *) &myAddr, sizeof(myAddr));
	if (i < 0) {
		printf("Couldn't bind socket\n");
		exit(-1);
	}

	/* listen */
	i = listen(myListenSocket, 5);
	if (i < 0) {
		printf("Couldn't listen\n");
		exit(-1);
	}

	/* wait for connection request */
	puts("Waiting for another a user to connect");
	addrSize = sizeof(clientAddr);

	clientSocket = accept(myListenSocket, (struct sockaddr *) &clientAddr,
			&addrSize);
	if (clientSocket < 0) {
		printf("Couldn't accept the connection\n");
		exit(-1);
	}

	close(myListenSocket);

	return clientSocket;

}

/**
 * Connect to the given remote machine
 * @param serverIp The IP of the remote machine to connect to
 * @param port The port to connect to
 * @return The connection to the remote machine
 */
int socketClient(char *serverIp, int port) {

	int mySocket;
	struct sockaddr_in addr;
	int i;

	/* create socket */
	mySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mySocket < 0) {
		printf("Couldn't open socket\n");
		exit(-1);
	}

	/* setup address */
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(serverIp);
	addr.sin_port = htons((unsigned short) port);

	/* connect to server */

	i = connect(mySocket, (struct sockaddr *) &addr, sizeof(addr));
	if (i < 0) {
		printf("Client could not connect!\n");
		exit(-1);
	}

	return mySocket;

}

/**
 * Close port and exit program with provided socket and exit status,
 * respectively.
 * @param connection The socket to close
 * @param exitStatus The exit code to exit the program with
 */
void exitSequence(int connection, int exitStatus) {
	close(connection);
	exit(exitStatus);
}

/**
 * Get a guess as a string from stdin
 * @param buffer The char string to set
 * @param guesses The current number of guesses
 */
void getGuess(char *buffer, int guesses) {

	printf("Ask question %d:\n", guesses);
	fgets(buffer, BUFFER_SIZE, stdin);
	buffer[strlen(buffer) - 1] = '\0';
}

/**
 * Get a char string from stdin
 * @param buffer The char string to set
 */
void getInput(char *buffer) {
	fgets(buffer, BUFFER_SIZE, stdin);
	buffer[strlen(buffer) - 1] = '\0';
}

/**
 * Prompts the user to continue the game or exit
 * @return CONTINUE to continue playing the game, or EXIT to exit the game
 */
endgame exitOrContinue() {
	char input[BUFFER_SIZE];

	do {
		puts("Type 'continue' to continue, or 'quit' to quit");
		getInput(input);

		if (strcmp(input, "continue") == 0) {
			return CONTINUE;
		} else if (strcmp(input, "quit") == 0) {
			return EXIT;
		} else {
			continue;
		}
	} while (1);
}

/**
 * Main method. This is where the main program loop is caged.
 * @param argc Number of arguments
 * @param argv List of arguments
 * @return 0 for good, anything else is bad
 */
int main(int argc, char **argv) {
	int guesses = 1;
	result guessResult;
	gamemode gameState = INVALID; // keep track of what duties this instance has
	int connection = 0; // holds socket to remote machine

	printDatArt();

	if (argc == 1) { // no arguments, wait for connection, is oracle
		connection = socketServer(SERVER_PORT);
		gameState = ORACLE;
		puts("Starting Oracle");

	} else if (argc == 2) { // one argument, connect to given ip at default port, is guesser
		connection = socketClient(argv[1], SERVER_PORT);
		gameState = GUESSER;
		puts("Starting Guesser");

	} else if (argc == 3) { // two arguments, connect to given ip at given port, is guesser
		connection = socketClient(argv[1], atoi(argv[2]));
		gameState = GUESSER;
		puts("Starting Guesser");

	} else {
		puts("Invalid arguments");
		return -1;
	}

	while (1) { // main game loop

		if (gameState == ORACLE) {
			receiveGuess(connection, guesses); // wait for and print question
			enableSignals();
			puts("Waiting for other player...");
			guessResult = checkForSignal(connection); // either WIN for win or NONE for everything else
			disableSignals();

			if (guessResult == WIN) { // Oracle says the guess is correct
				puts("Guesser Wins, waiting for Guesser to continue or quit");
				char message[BUFFER_SIZE];
				getMessage(message, connection);

				if (strcmp(message, "EXIT") == 0) { // exit program if guesser says so
					puts("Guesser quit, You're still the Oracle");
					guesses = 1;
					guessResult = NONE;
					close(connection);
					connection = socketServer(SERVER_PORT);

				} else if (strcmp(message, "CONTINUE") == 0) { // continue playing if guesser says so
					puts("Guesser continues the game");
					gameState = GUESSER;
					guesses = 1;
					guessResult = NONE;
				}
			} else {
				// increase guess count if guesser does not win
				guesses++;
			}

			// check if guesser has guessed too many times
			if (guesses > MAX_GUESSES) { // Oracle wins
				puts("Oracle Wins! Waiting for your response");
				endgame quit = exitOrContinue(); // prompt user to continue or exit

				if (quit == EXIT) { // if oracle wants to exit
					puts("You opted to exit");
					sendMessage(connection, "EXIT");
					exitSequence(connection, 0);

				} else { // if oracle wants to continue
					puts("You opted to continue");
					sendMessage(connection, "CONTINUE");
					gameState = ORACLE;
					guesses = 1;
					guessResult = NONE;
				}
			}

		} else if (gameState == GUESSER) {
			char guess[BUFFER_SIZE];
			getGuess(guess, guesses); // get guess from stdin
			sendMessage(connection, guess); // send guess to oracle
			puts("Waiting for other player...");
			guessResult = receiveResult(connection); // Returns YES if oracle says yes, NO if no, WIN for win, and NONE for error

			if (guessResult == WIN) { // if oracle says that guesser wins
				puts("Guesser wins!");
				endgame quit = exitOrContinue(); // prompt guesser to continue or exit game

				if (quit == EXIT) { // if guesser wants to exit
					puts("You opted to exit");
					sendMessage(connection, "EXIT");
					exitSequence(connection, 0);

				} else { // if guesser wants to continue
					puts("You opted to continue");
					sendMessage(connection, "CONTINUE");
					gameState = ORACLE;
					guesses = 1;
					guessResult = NONE;
				}

			} else if (guessResult == YES) { // close or incorrect guess, just increment guesses
				guesses++;

			} else if (guessResult == NO) { // close or incorrect guess, just increment guesses
				guesses++;
			}

			if (guesses > MAX_GUESSES) { // guesser loses, waits for response
				puts("You lose, waiting on Oracle's response");
				char message[BUFFER_SIZE];
				getMessage(message, connection);

				if (strcmp(message, "EXIT") == 0) { // oracle quits
					puts("Oracle quit, You're now the Oracle");
					gameState = ORACLE;
					guesses = 1;
					guessResult = NONE;
					close(connection);
					connection = socketServer(SERVER_PORT);

				} else if (strcmp(message, "CONTINUE") == 0) { // oracle tells guesser to keep playing
					puts("Oracle opted to continue");
					gameState = GUESSER;
					guesses = 1;
					guessResult = NONE;
				}
			}
		}

	}
	// should never reach this*
	return 0;
}
// [*] You never know
