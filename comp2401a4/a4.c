/*
 * a4.c
 *
 *  Created on: Nov 12, 2013
 *      Author: Jon Simpson
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a4Defs.h"

// Initialise global variable for writing to a file
FILE *outputFile;

void clearStdin() {
	int ch;
	while ((ch = fgetc(stdin)) != EOF && ch != '\n') {
	}
}

char* getGenre(GenreType genre) {
	switch (genre) {
	case C_ACTION:
		return "Action";
	case C_ADVENTURE:
		return "Adventure";
	case C_COMEDY:
		return "Comedy";
	case C_DRAMA:
		return "Drama";
	case C_HORROR:
		return "Horror";
	case C_SF:
		return "Science-Fiction";
	case C_WESTERN:
		return "Western";
	default:
		return NULL;
	}
}

int getIntBit(int x, int n) {
	return ((x & (1 << n)) >> n);
}

unsigned char setBit(unsigned char c, int n) {
	return (c | (1 << n));
}

unsigned char clearBit(unsigned char c, int n) {
	return (c & (~(1 << n)));
}

/*   Function:  dumpBytes                     */
/*         in:  byte array to be output       */
/*         in:  number of bytes in byte array */
/*    Purpose:  output byte array as hex to outputFile stream      */

void dumpBytes(char *b, int n) {
	int k;

	for (k = n - 1; k >= 0; --k) {
		fprintf(outputFile, "%02x ", (unsigned char) b[k]);
	}
	fprintf(outputFile, "\n");
}

/*   Function:  convertToBytes                     */
/*         in:  int to be converted to byte array  */
/*        out:  resulting byte array               */
/*    Purpose:  separate int into 4 bytes          */

void convertToBytes(int num, unsigned char *bytes) {
	int i, j;
	unsigned char tmpChar;

	for (i = 0; i < MAX_BYTES; ++i) {
		tmpChar = 0;
		for (j = 0; j < 8; ++j) {
			if (getIntBit(num, j + (i * 8)) == 0)
				tmpChar = clearBit(tmpChar, j);
			else
				tmpChar = setBit(tmpChar, j);
		}
		bytes[i] = tmpChar;
	}
}

void dumpList(MovieNodeType *list) {

	if (list == NULL) {
		return;
	}

	unsigned char bytes[MAX_BYTES];

	fprintf(outputFile, "     ------------ LIST ------------\n");
	fprintf(outputFile, "      -- head:    ");
	convertToBytes((int) list, bytes);
	dumpBytes(bytes, MAX_BYTES);

	while (list != NULL) {

		fprintf(outputFile, "     -----------  node addr:  ");
		convertToBytes((int) list, bytes);
		dumpBytes(bytes, MAX_BYTES);

		fprintf(outputFile, "     -----------              data:  ");
		convertToBytes((int) list->data, bytes);
		dumpBytes(bytes, MAX_BYTES);

		fprintf(outputFile,
				"     -----------                     -- title: %s\n",
				list->data->title);
		fprintf(outputFile,
				"     -----------                     -- year:  %d\n",
				list->data->year);
		fprintf(outputFile,
				"     -----------                     -- genre: %s\n",
				getGenre(list->data->genre));

		fprintf(outputFile, "     -----------              prev:  ");
		convertToBytes((int) list->prev, bytes);
		dumpBytes(bytes, MAX_BYTES);

		fprintf(outputFile, "     -----------              next:  ");
		convertToBytes((int) list->next, bytes);
		dumpBytes(bytes, MAX_BYTES);

		list = list->next;
	}

	fprintf(outputFile, "     -------- END OF LIST --------\n");

}

void printMovieData(MovieNodeType *list) {

	while (list != NULL) {
		printf("%s : %d %s\n", list->data->title, list->data->year,
				getGenre(list->data->genre));
		list = list->next;

	}

}

void printMoviesByGenre(MovieNodeType *list, char *genreInput) {

	MovieNodeType *newList = NULL;
	GenreType genre;

	if (strcmp(genreInput, "comedy") == 0) {
		genre = C_COMEDY;
	} else if (strcmp(genreInput, "drama") == 0) {
		genre = C_DRAMA;
	} else if (strcmp(genreInput, "action") == 0) {
		genre = C_ACTION;
	} else if (strcmp(genreInput, "horror") == 0) {
		genre = C_HORROR;
	} else if (strcmp(genreInput, "sf") == 0) {
		genre = C_SF;
	} else if (strcmp(genreInput, "adventure") == 0) {
		genre = C_ADVENTURE;
	} else if (strcmp(genreInput, "western") == 0) {
		genre = C_WESTERN;
	} else {
		puts("Invalid genre inputed");
		return;
	}

	while (list != NULL) {
		if (list->data->genre == genre) {

			addToMovieList(&newList, list->data);

		}
		list = list->next;
	}

	// output nodes
	printMovieData(newList);

	// free nodes
	cleanupMovieNodes(list);

}

/**
 * Secretly not used. heheh
 * @param movieNode
 * @param name
 */
void deleteMovie(MovieNodeType **movieNode, char *name) {

	// if the movieNode is invalid: return
	if (movieNode == NULL || *movieNode == NULL) {
		return;
	}

	MovieNodeType *node = *movieNode;

	// loop through the list until it finds the movie, or reaches the end
	do {
		if (strcmp(node->data->title, name) != 0) {
			if (node->next == NULL) {
				return;
			} else {
				node = node->next;
			}
			continue;
		} else {

			// if it is the only node in the list
			if (node->prev == NULL && node->next == NULL) {
				*movieNode = NULL;
				freeNodeAndData(node);
				return;

				// if it is the first node in the list
			} else if (node->prev == NULL) {
				node->next->prev = NULL;
				*movieNode = node->next;
				freeNodeAndData(node);
				return;

				// if it is the last node in the list
			} else if (node->next == NULL) {
				node->prev->next = NULL;
				freeNodeAndData(node);
				return;

				// if it is in the middle of the list
			} else {
				node->prev->next = node->next;
				node->next->prev = node->prev;
				freeNodeAndData(node);
				return;
			}
		}
	} while (node != NULL);

}

void initMovie(MovieType **movie, char* name, int year, GenreType genre) {

	MovieType *newMovie = (MovieType *) malloc(sizeof(MovieType));
	strcpy(newMovie->title, name);
	newMovie->year = year;
	newMovie->genre = genre;

	*movie = newMovie;
}

void getMovieData(MovieNodeType **movieList) {

	fprintf(outputFile, "*Entering getMovieData*\n");
	dumpList(*movieList);

	/*
	 * Ask the user how many movies they want to input
	 */
	int returnVal, numMovies;
	const int SCANF_UNSUCCESSFUL = 1;

	do {
		printf("Enter the number of movies to be entered: ");
		returnVal = scanf("%d", &numMovies);
		clearStdin();

	} while (returnVal != SCANF_UNSUCCESSFUL);

	/*
	 * Get the user to input the movie data
	 */
	int i = 0;
	for (i = 0; i < numMovies; ++i) {
		char title[MAX_STR];
		int year;
		GenreType genre;

		// get movie name
		printf("Enter the name of movie %d:\n", i + 1);

		fgets(title, MAX_STR, stdin);
		title[strlen(title) - 1] = '\0';

		printf("Movie name: %s\n", title);

		// get movie year
		do {
			printf("Enter the year of movie %d:\n", i + 1);
			returnVal = scanf("%d", &year);
			clearStdin();

		} while (returnVal != SCANF_UNSUCCESSFUL);

		printf("movie year: %d\n", year);

		// get movie genre
		char genreInput[MAX_STR];
		do {
			printf("Enter the genre of movie %d:\n", i + 1);

			fgets(genreInput, MAX_STR, stdin);
			genreInput[strlen(genreInput) - 1] = '\0';

			if (strcmp(genreInput, "comedy") == 0) {
				genre = C_COMEDY;
				break;
			} else if (strcmp(genreInput, "drama") == 0) {
				genre = C_DRAMA;
				break;
			} else if (strcmp(genreInput, "action") == 0) {
				genre = C_ACTION;
				break;
			} else if (strcmp(genreInput, "horror") == 0) {
				genre = C_HORROR;
				break;
			} else if (strcmp(genreInput, "sf") == 0) {
				genre = C_SF;
				break;
			} else if (strcmp(genreInput, "adventure") == 0) {
				genre = C_ADVENTURE;
				break;
			} else if (strcmp(genreInput, "western") == 0) {
				genre = C_WESTERN;
				break;
			} else {
				continue;
			}

		} while (1);

		printf("Movie genre: \n");

		// add movie to list
		MovieType *movie;
		initMovie(&movie, title, year, genre);
		addToMovieList(movieList, movie);
	}

	dumpList(*movieList);
	fprintf(outputFile, "*Leaving getMovieData*\n");
}

void mainMenu(int *selection) {

	puts(" __   __  _______  __   __  ___   _______                               ");
	puts("|  |_|  ||       ||  | |  ||   | |       |                              ");
	puts("|       ||   _   ||  |_|  ||   | |    ___|                              ");
	puts("|       ||  | |  ||       ||   | |   |___                               ");
	puts("|       ||  |_|  ||       ||   | |    ___|                              ");
	puts("| ||_|| ||       | |     | |   | |   |___                               ");
	puts("|_|   |_||_______|  |___|  |___| |_______|                              ");
	puts(" ______   _______  _______  _______  _______  _______  _______  _______ ");
	puts("|      | |   _   ||       ||   _   ||  _    ||   _   ||       ||       |");
	puts("|  _    ||  |_|  ||_     _||  |_|  || |_|   ||  |_|  ||  _____||    ___|");
	puts("| | |   ||       |  |   |  |       ||       ||       || |_____ |   |___ ");
	puts("| |_|   ||       |  |   |  |       ||  _   | |       ||_____  ||    ___|");
	puts("|       ||   _   |  |   |  |   _   || |_|   ||   _   | _____| ||   |___ ");
	puts("|______| |__| |__|  |___|  |__| |__||_______||__| |__||_______||_______|");

	puts("");
	puts("Enter a number from the list");

	puts("(1) Add movies");
	puts("(2) Delete a movie");
	puts("(3) List all movies");
	puts("(4) List all movies by genre");
	puts("(0) Quit");

	// get the menu selection from the user
	int returnVal, input = -1;

	do {
		returnVal = scanf("%d", &input);
		clearStdin();

	} while (returnVal != 1);

	*selection = input;

}

void menuDeleteMovie(MovieNodeType **list) {

	printf("Enter the name of the movie to remove:\n");
	char title[MAX_STR];

	fgets(title, MAX_STR, stdin);
	title[strlen(title) - 1] = '\0';

	int result = removeByName(list, title);

	if (result == OK) {
		puts("Successfully removed the movie");
	} else {
		printf("Failed to remove: %s\n", title);
	}

}

void menuPrintMoviesByGenre(MovieNodeType *list) {

	char title[MAX_STR];

	puts("Please enter the genre to show:");
	fgets(title, MAX_STR, stdin);
	title[strlen(title) - 1] = '\0';

	printMoviesByGenre(list, title);
}

void cleanupMovieNodes(MovieNodeType *list) {

	MovieNodeType *next = NULL;

	while (list != NULL) {
		next = list->next;
		free(list);
		list = next;
	}

}

void cleanupMovies(MovieNodeType *list) {

	MovieNodeType *next = NULL;

	while (list != NULL) {
		next = list->next;
		free(list->data);
		free(list);
		list = next;
	}
}

void cleanupProgram(MovieNodeType *list) {
	// close outputFile
	fclose(outputFile);
	cleanupMovies(list);
}

int main(void) {

	system("clear");

	// open the output file
	outputFile = fopen("a4output.txt", "w");

	// set the library's buffer to zero. Writes straight to file
	setbuf(outputFile, NULL);

	if (!outputFile) {
		printf("Error opening file\n");
		exit(ERR_OPENING_FILE);
	}

	int i;
	MovieNodeType *list = NULL;
	printMovieData(list);

	while (1) {

		mainMenu(&i);

		if (i == 0) {
			cleanupProgram(list);
			return OK;
		} else if (i == 1) {
			getMovieData(&list);
		} else if (i == 2) {
			menuDeleteMovie(&list);
		} else if (i == 3) {
			printMovieData(list);
		} else if (i == 4) {
			menuPrintMoviesByGenre(list);
		} else {
			continue;
		}
	}
}
