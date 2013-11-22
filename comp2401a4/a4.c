/*
 * a4.c
 *
 *  Created on: Nov 12, 2013
 *      Author: Jon Simpson
 *
 * This program manages a database of movies using a doubly-linked list. The user
 * is presented a menu to select a slew of operations (add, delete, view,
 * search-view, exit). Adding a movie requires entering the name, year and genre of
 * the movie. Movies are sorted in chronological order based on title. If there
 * are two or more movies with the same title then the matching movies are
 * sorted by chronologically by their year.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a4Defs.h"

// Initialise global variable for writing to a file
FILE *outputFile;

/**
 * Clears stdin to prevent multiple newlines appearing in stdout/output file
 */
void clearStdin() {
	int ch;
	while ((ch = fgetc(stdin)) != EOF && ch != '\n') {
	}
}

/**
 * returns a char array representing the passed GenreType
 * @param genre The genre type to get a char array of
 * @return The genre in a char array format
 */
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

/**
 * returns the value of n in x
 * @param x
 * @param n
 * @return
 */
int getIntBit(int x, int n) {
	return ((x & (1 << n)) >> n);
}

/**
 * Sets the n'th bit of c to 1
 * @param c
 * @param n
 * @return
 */
unsigned char setBit(unsigned char c, int n) {
	return (c | (1 << n));
}

/**
 * Sets the n'th bit of c to 0
 * @param c
 * @param n
 * @return
 */
unsigned char clearBit(unsigned char c, int n) {
	return (c & (~(1 << n)));
}

/**
 *   Function:  dumpBytes
 *         in:  byte array to be output
 *         in:  number of bytes in byte array
 *    Purpose:  output byte array as hex to outputFile stream
 */
void dumpBytes(char *b, int n) {
	int k;

	for (k = n - 1; k >= 0; --k) {
		fprintf(outputFile, "%02x ", (unsigned char) b[k]);
	}
	fprintf(outputFile, "\n");
}

/**
 *   Function:  convertToBytes
 *         in:  int to be converted to byte array
 *        out:  resulting byte array
 *    Purpose:  separate int into 4 bytes
 */
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

/**
 * Output the given list of moviesto the file specified by the outputFile global
 * variable
 * @param list The list to output
 */
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

/**
 * Prints the given list of movies to stdout
 * @param list The list to print
 */
void printMovieData(MovieNodeType *list) {

	puts("List of movies");
	while (list != NULL) {
		printf("Title: %s     Year: %d     Genre: %s\n", list->data->title,
				list->data->year, getGenre(list->data->genre));
		list = list->next;

	}

	puts("");

}

/**
 * Prints out a list of movies to stdout that match the same genre specified by
 * genreInput
 * @param list The list to search through
 * @param genreInput The genre to match
 */
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
	} else if (strcmp(genreInput, "science-fiction") == 0) {
		genre = C_SF;
	} else if (strcmp(genreInput, "adventure") == 0) {
		genre = C_ADVENTURE;
	} else if (strcmp(genreInput, "western") == 0) {
		genre = C_WESTERN;
	} else {
		puts("Invalid genre inputed");
		return;
	}

	MovieNodeType *temp = list;

	while (temp != NULL) {
		if (temp->data->genre == genre) {

			addToMovieList(&newList, temp->data);

		}
		temp = temp->next;
	}

	// output nodes
	printMovieData(newList);

	// free nodes
	cleanupMovieNodes(newList);

}

/**
 * Deletes the first occurrence of name that matches the movie's title from the
 * given list
 * @param movieNode The list of movies to find and remove the node from
 * @param name The name of the movie to remove
 */
void deleteMovie(MovieNodeType **list, char *title) {

	fprintf(outputFile, "* Entering DeleteMovie *\n");
	dumpList(*list);

	MovieNodeType *current = *list;

	while (current != NULL) {
		int compare = strcmp(current->data->title, title);
		if (compare == 0) {

			// if removing from the beginning
			if (current->prev == NULL) {
				*list = current->next;
			} else {
				current->prev->next = current->next;
			}

			// if removing from the end
			if (current->next != NULL) {
				current->next->prev = current->prev;
			}
			current->next = NULL;
			current->prev = NULL;

			freeNodeAndData(current);

			dumpList(*list);
			fprintf(outputFile, "* Leaving DeleteMovie *\n");
			return;

		} else {
			current = current->next;
		}
	}

	dumpList(*list);
	fprintf(outputFile, "* Leaving DeleteMovie *\n");
	return;

}

/**
 * Initialise a movie pointer specified by movie with the given name, year and
 * genre
 * @param movie The new movie pointer to return
 * @param name Name to use
 * @param year Year to use
 * @param genre Genre to use
 */
void initMovie(MovieType **movie, char* name, int year, GenreType genre) {

	MovieType *newMovie = (MovieType *) malloc(sizeof(MovieType));
	strcpy(newMovie->title, name);
	newMovie->year = year;
	newMovie->genre = genre;

	*movie = newMovie;
}

/**
 * Asks the user to add a user specified amount of movies and their respective
 * data. Adds all movies to the specified movieList
 * @param movieList The list to add movies to
 */
void getMovieData(MovieNodeType **movieList) {

	fprintf(outputFile, "* Entering getMovieData *\n");
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

	} while (returnVal != SCANF_UNSUCCESSFUL || numMovies < 0);

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

		// get movie year
		do {
			printf("Enter the year of movie %d:\n", i + 1);
			returnVal = scanf("%d", &year);
			clearStdin();

		} while (returnVal != SCANF_UNSUCCESSFUL || year < 0);

		// get movie genre
		char genreInput[MAX_STR];
		do {
			printf(
					"Enter the genre of movie %d\nAvailable choices are 'comedy', 'drama', 'action', 'horror', 'science-fiction', 'adventure', 'western':\n",
					i + 1);

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
			} else if (strcmp(genreInput, "science-fiction") == 0) {
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

		// add movie to list
		MovieType *movie;
		initMovie(&movie, title, year, genre);
		addToMovieList(movieList, movie);
	}

	dumpList(*movieList);
	fprintf(outputFile, "* Leaving getMovieData *\n");
}

/**
 * Prints out a menu to stdout for the user to interact with
 * @param selection The number that represents which menu command to run
 */
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

	puts("\nEnter a number from the list");

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

/**
 * A little submenu to get the name of the movie to remove, then calls the
 * deleteMovie function with the required parameters
 * @param list The list to delete an item from
 */
void menuDeleteMovie(MovieNodeType **list) {

	printf("Enter the name of the movie to remove:\n");
	char title[MAX_STR];

	fgets(title, MAX_STR, stdin);
	title[strlen(title) - 1] = '\0';

	deleteMovie(list, title);

}

/**
 * A little submenu to get the genre of the movies that the user wants
 * outputted, then calls printMoviesByGenre
 * @param list The list of movies to search throuh
 */
void menuPrintMoviesByGenre(MovieNodeType *list) {

	char title[MAX_STR];

	puts("Please enter the genre to show");
	puts("Available choices are 'comedy', 'drama', 'action', 'horror', 'science-fiction', 'adventure', 'western':");
	fgets(title, MAX_STR, stdin);
	title[strlen(title) - 1] = '\0';

	printMoviesByGenre(list, title);
}

/**
 * Deallocate only the nodes in the passed list
 * @param list The list to deallocate all of the nodes
 */
void cleanupMovieNodes(MovieNodeType *list) {

	MovieNodeType *next = NULL;

	while (list != NULL) {
		next = list->next;
		free(list);
		list = next;
	}

}

/**
 * Deallocate both the nodes and data in the given list
 * @param list The list to deallocate from
 */
void cleanupMovies(MovieNodeType *list) {

	MovieNodeType *next = NULL;

	while (list != NULL) {
		next = list->next;
		free(list->data);
		free(list);
		list = next;
	}
}

/**
 * Prepares this program for exiting. Closes the open file and deallocates the
 * given list of movies
 * @param list The list of movies to deallocate
 */
void cleanupProgram(MovieNodeType *list) {
	// close outputFile
	fclose(outputFile);
	cleanupMovies(list);
}

/**
 * Main function of this program. Opens the output file, and shows the main
 * menu to the user
 * @return 0 or OK for success, any other number for failure
 */
int main(void) {

	system("clear");

	// open the output file
	outputFile = fopen("a4output.txt", "w");

	// set the library's file output buffer to zero. Writes straight to file
	setbuf(outputFile, NULL);

	if (!outputFile) {
		printf("Error opening file\n");
		exit(ERR_OPENING_FILE);
	}

	int menuChoice = -1;
	MovieNodeType *list = NULL;

	while (1) {

		mainMenu(&menuChoice);

		if (menuChoice == 0) {
			cleanupProgram(list);
			return OK;
		} else if (menuChoice == 1) {
			getMovieData(&list);
		} else if (menuChoice == 2) {
			menuDeleteMovie(&list);
		} else if (menuChoice == 3) {
			printMovieData(list);
		} else if (menuChoice == 4) {
			menuPrintMoviesByGenre(list);
		} else {
			continue;
		}
	}
}
