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

#define SERVER_PORT 60002
#define BUFFER_SIZE 120

typedef enum {NONE, YES, NO, WIN} result;
typedef enum {INVALID, ORACLE, GUESSER} gamemode;

// global variable for keeping track of which signal was run
// https://culearn.carleton.ca/moodle/mod/forum/discuss.php?d=64507
volatile result action = NONE;

void sendMessage(int socket, char *buffer) {
	send(socket, buffer, strlen(buffer), 0);
}

void handleYes(int i) {
	action = YES;
}

void handleNo(int i) {
	action = NO;
}

void handleWin(int i) {
	action = WIN;
}

void receiveGuess(int connection) {
	int bytesRcv;
	char buffer[BUFFER_SIZE];

	bytesRcv = recv(connection, buffer, sizeof(buffer), 0);
	buffer[bytesRcv] = 0;
	printf("Question:  %s\n", buffer);
}

/**
 * Used by guesser
 * @param connection
 * @return
 */
char receiveResult(int connection) {
	int bytesRcv;
	char buffer[BUFFER_SIZE];

	bytesRcv = recv(connection, buffer, sizeof(buffer), 0);
	buffer[bytesRcv] = 0;

	if (strcmp(buffer, "Yes") == 0) { // yes condition
		return YES;
	} else if (strcmp(buffer, "No") == 0) { // no condition
		return NO;
	} else if (strcmp(buffer, "Win!") == 0) { // win condition
		return WIN;
	} else {
		fputs("Received invalid response from Oracle", stderr);
		return NONE;
	}
}

void startGuesser(char *serverIp, int serverPort) {
	int mySocket, bytesRcv;
	struct sockaddr_in addr;
	int i;

	char inStr[80];
	char buffer[80];

	/* create socket */
	mySocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mySocket < 0) {
		printf("eek! couldn't open socket\n");
		exit(-1);
	}

	/* setup address */
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(serverIp);
	addr.sin_port = htons((unsigned short) SERVER_PORT);

	/* connect to server */

	i = connect(mySocket, (struct sockaddr *) &addr, sizeof(addr));
	if (i < 0) {
		printf("client could not connect!\n");
		exit(-1);
	}

	/* get input from user and send to server */
	while (1) {
		printf("what is your message?  ");
		//    scanf("%s", inStr);
		gets(inStr);
		strcpy(buffer, inStr);
		send(mySocket, buffer, strlen(buffer), 0);

		if (strcmp(inStr, "quit") == 0) {
			break;
		}

		/* read message from client and do something with it */
		while (1) {
			bytesRcv = recv(mySocket, buffer, sizeof(buffer), 0);
			buffer[bytesRcv] = 0;
			printf("this is what the client sent:  %s\n", buffer);
			if (strcmp(buffer, "quit") == 0) {
				break;
			}
		}
	}

	/* close the socket */

	close(mySocket);
}

/**
 * waits for the user to enter Ctrl-C, Ctrl-Z or Ctrl-\
 * @param clientSocket
 */
char checkForSignal(int clientSocket) {
	while (action == NONE) {
	}

	switch (action) {
	case YES:
		sendMessage(clientSocket, "Yes");
		action = NONE;
		return 0;
		break;
	case NO:
		sendMessage(clientSocket, "No");
		action = NONE;
		return 0;
		break;
	case WIN:
		sendMessage(clientSocket, "Win!");
		action = NONE;
		return 1;
		break;
	default:
		break;
	}
	// TODO add a return
}

void enableSignals() {
	signal(SIGINT, handleYes);
	signal(SIGTSTP, handleNo);
	signal(SIGQUIT, handleWin);
}

void disableSignals() {
	signal(SIGINT, NULL);
	signal(SIGTSTP, NULL);
	signal(SIGQUIT, NULL);
}

//void startOracle(int serverPort) {
//
//	// register signals only when guesser connects
//	enableSignals();
//	// fork and enter checkForSignal
//	if (fork() == 0) {
//		checkForSignal(clientSocket);
//	}
//
//	/* read message from client and do something with it */
//	while (1) {
//		bytesRcv = recv(clientSocket, buffer, sizeof(buffer), 0);
//		buffer[bytesRcv] = 0;
//		printf("this is what the client sent:  %s\n", buffer);
//		if (strcmp(buffer, "quit") == 0) {
//			break;
//		}
//	}
//
//	/* close sockets */
//
////	close(myListenSocket);
//	close(clientSocket);
//}

int socketListener(int serverPort) {

	int myListenSocket, clientSocket;
	struct sockaddr_in myAddr, clientAddr;
	int i, addrSize, bytesRcv;

	char buffer[30];

	/* create socket */

	myListenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (myListenSocket < 0) {
		printf("eek! couldn't open socket\n");
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
		printf("eek! couldn't bind socket\n");
		exit(-1);
	}

	/* listen */
	i = listen(myListenSocket, 5);
	if (i < 0) {
		printf("eek! couldn't listen\n");
		exit(-1);
	}

	/* wait for connection request */

	addrSize = sizeof(clientAddr);

	clientSocket = accept(myListenSocket, (struct sockaddr *) &clientAddr,
			&addrSize);
	if (clientSocket < 0) {
		printf("eek! couldn't accept the connection\n");
		exit(-1);
	}

	close(myListenSocket);

	return clientSocket;

}

void exitSequence(int connection, int exitStatus) {
	close(connection);
	exit(exitStatus);
}

void getGuess(char *result) {

	fgets(result, BUFFER_SIZE, stdin);
	result[strlen(result) - 1] = '\0';
}

int main(int argc, char **argv) {
	int guesses = 0, result = 0;
	gamemode gameState = INVALID; // set to invalid game state
	int connection = 0;

	if (argc == 1) { // no arguments, wait for connection, is oracle
		connection = socketListener(SERVER_PORT);
		gameState = ORACLE;

	} else if (argc == 2) { // one argument, connect to given ip at default port, is guesser
		startGuesser(argv[1], SERVER_PORT);
		gameState = GUESSER;

	} else if (argc == 3) { // two arguments, connect to given ip at given port, is guesser
		startGuesser(argv[1], atoi(argv[2]));
		gameState = GUESSER;

	} else { // invalid arguments
		puts("Invalid arguments");
		return -1;
	}

	while (1) { // program loop
		if (gameState == ORACLE) { // oracle
			receiveGuess(connection);
			enableSignals();
			result = checkForSignal(connection);
			disableSignals();

		} else if (gameState == GUESSER) { // guesser
			char guess[BUFFER_SIZE];
			getGuess(guess);
			sendMessage(connection, guess);
			result = receiveGuess(connection);

			if (result == 1) {

			}

		} else { // invalid game state
			// quit the program
			exitSequence(connection, -1);
		}
		close(socket);
	}

	return 0;
}

