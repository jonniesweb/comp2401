/*
 * a4.c
 *
 *  Created on: Nov 12, 2013
 *      Author: jon
 */

#include "a4Defs.h"
#include "a4Stack.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clearStdin() {
	int ch;
	while ((ch = fgetc(stdin)) != EOF && ch != '\n') {
	}
}

void printMovieData(MovieNodeType *list) {
	while (list != NULL) {
		printf("%s : %d\n", list->data->title, list->data->year);
		list = list->next;

	}
}

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

void initMovie(MovieNodeType **node, char* name, int year, GenreType genre) {

	MovieType *movie = (MovieType *) malloc(sizeof(MovieType));
	strcpy(movie->title, name);
	movie->year = year;
	movie->genre = genre;

	MovieNodeType *movieNode = (MovieNodeType *) malloc(sizeof(MovieNodeType));
	movieNode->data = movie;
	movieNode->next = NULL;
	movieNode->prev = NULL;

	*node = movieNode;
}

void getMovieData(MovieNodeType **movieList) {

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
	 * Allocate space for the movies
	 */
	//	MovieType* movies = malloc(numMovies * sizeof(MovieType));
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
		MovieNodeType *node;
		initMovie(&node, title, year, genre);
		add(movieList, node);
	}
}

void mainMenu(int *selection) {
	puts("Movie Database\n");
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

void menuGetMoviesByGenre(MovieNodeType **list) {


}

int main(void) {

	int i;
	MovieNodeType *list = NULL;

	while (1) {

		mainMenu(&i);

		if (i == 0) {
			return OK;
		} else if (i == 1) {
			getMovieData(&list);
		} else if (i == 2) {
			menuDeleteMovie(&list);
		} else if (i == 3) {
			printMovieData(list);
		} else if (i == 4) {
			menuGetMoviesByGenre(&list);
		} else {
			continue;
		}
	}
}
