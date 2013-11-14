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

void deleteMovie(MovieNodeType **movieNode, char *name) {



}
# if 0
int main(void) {

	printf("%d\n", strcmp("f", "a"));
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
	MovieType *movies = malloc(numMovies * sizeof(MovieType));

	/*
	 * Get the user to input the movie data
	 */

	int i = 0;
	for (i = 0; i < numMovies; ++i) {

		// get movie name
		printf("Enter the name of movie %d:\n", i+1);

		fgets(movies[i].title, MAX_STR, stdin);
		movies[i].title[strlen(movies[i].title) - 1] = '\0';

		printf("Movie name: %s\n", movies[i].title);

		// get movie year
		do {
			printf("Enter the year of movie %d:\n", i+1);
			returnVal = scanf("%d", &(movies[i].year));
			clearStdin();

		} while (returnVal != SCANF_UNSUCCESSFUL);

		printf("movie year: %d\n", movies[i].year);

		// get movie genre
		char genreInput[MAX_STR];
		do {
			printf("Enter the genre of movie %d:\n", i+1);

			fgets(genreInput, MAX_STR, stdin);
			genreInput[strlen(genreInput) - 1] = '\0';

			if (strcmp(genreInput, "comedy") == 0) {
				movies[i].genre = C_COMEDY;
				break;
			} else if (strcmp(genreInput, "drama") == 0) {
				movies[i].genre = C_DRAMA;
				break;
			} else if (strcmp(genreInput, "action") == 0) {
				movies[i].genre = C_ACTION;
				break;
			} else if (strcmp(genreInput, "horror") == 0) {
				movies[i].genre = C_HORROR;
				break;
			} else if (strcmp(genreInput, "sf") == 0) {
				movies[i].genre = C_SF;
				break;
			} else if (strcmp(genreInput, "adventure") == 0) {
				movies[i].genre = C_ADVENTURE;
				break;
			} else if (strcmp(genreInput, "western") == 0) {
				movies[i].genre = C_WESTERN;
				break;
			} else {
				continue;
			}

		} while (1);

		printf("Movie genre: \n");




	}




	return 0;

}
#endif
