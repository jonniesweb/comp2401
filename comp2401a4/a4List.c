/*
 * a4List.c
 *
 *  Created on: Nov 14, 2013
 *      Author: Jon Simpson
 *
 * Contains various functions related to using a doubly-linked list
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "a4Defs.h"

/**
 * Deallocates the passed node
 * @param node Deallocates this node
 */
void freeNode(MovieNodeType *node) {
	free(node);
}

/**
 * Deallocates and frees the passed node
 * @param node Deallocates this node and its data
 */
void freeNodeAndData(MovieNodeType *node) {
	free(node->data);
	free(node);
}

/**
 * Adds newNode to the right of node
 * @param node
 * @param newNode
 */
void addToRight(MovieNodeType *node, MovieNodeType *newNode) {
	newNode->next = node->next;
	newNode->prev = node;
	node->next->prev = newNode;
	node->next = newNode;
}

/**
 * Adds newNode to the left of node
 * @param node
 * @param newNode
 */
void addToLeft(MovieNodeType *node, MovieNodeType *newNode) {
	node->prev->next = newNode;
	newNode->prev = node->prev;
	node->prev = newNode;
	newNode->next = node;
}

/**
 * Properly adds a node to an empty list
 * @param list
 * @param node
 */
void addToEmptyList(MovieNodeType** list, MovieNodeType* node) {
	*list = node;
	node->prev = NULL;
	node->next = NULL;
}

/**
 * Adds the given movie to the list of movies
 * @param list
 * @param movie
 */
void addToMovieList(MovieNodeType** list, MovieType *movie) {

	fprintf(outputFile, "* Entering addToMovieList *\n");
	dumpList(*list);

	MovieNodeType *node = malloc(sizeof(MovieNodeType));
	node->data = movie;

	// If list is empty
	if (*list == NULL) {
		addToEmptyList(list, node);

		dumpList(*list);
		fprintf(outputFile, "* Leaving addToMovieList *\n");
		return;

	} else {
		MovieNodeType *current = *list;
		do {
			int comparison = strcmp(node->data->title, current->data->title);
			if (comparison > 0) {
				// advance to the next node

				if (current->next == NULL) {
					// add to the end
					current->next = node;
					node->next = NULL;
					node->prev = current;

					dumpList(*list);
					fprintf(outputFile, "* Leaving addToMovieList *\n");
					return;

				} else {
					current = current->next;
					continue;
				}

				// if the titles are equal
			} else if (comparison == 0) {
				// compare the years
				if (current->data->year > node->data->year) {
					if (current->prev == NULL) {
						// add to beginning
						node->next = *list;
						node->prev = NULL;
						*list = node;

					} else {
						addToLeft(current, node);
					}

					dumpList(*list);
					fprintf(outputFile, "* Leaving addToMovieList *\n");
					return;
				}
				current = current->next;
				continue;

			} else if (comparison < 0) {

				if (current->prev == NULL) {
					current->prev = node;
					node->next = current;
					node->prev = NULL;
					*list = node;
					return;

				} else {
					addToLeft(current, node);

					dumpList(*list);
					fprintf(outputFile, "* Leaving addToMovieList *\n");
					return;
				}
			}

		} while (1);
	}

	dumpList(*list);
	fprintf(outputFile, "* Leaving addToMovieList *\n");

}

/*
 * Test function for a4Stack.c
 */
#if 0
#include <stdio.h>
#include <stdlib.h>

void initNode(MovieNodeType** node, char *title, GenreType genre, int year) {
	*node = (MovieNodeType*) malloc(sizeof(MovieNodeType));
	(*node)->data = (MovieType*) malloc(sizeof(MovieType));
	(*node)->data->genre = genre;
	strcpy((*node)->data->title, title);
	(*node)->data->year = year;
}

int main(void) {

	MovieNodeType *list = NULL;

	MovieNodeType *a;
	MovieNodeType *b;
	MovieNodeType *c;

	initNode(&a, "a", C_ACTION, 1994);
	initNode(&b, "a", C_HORROR, 1984);
	initNode(&c, "a", C_SF, 2013);

	add(&list, c);
	add(&list, b);
	add(&list, a);

	/*
	 * Output list
	 */
	MovieNodeType *current = list;
	while (current != NULL) {
		printf("%s : %d\n", current->data->title, current->data->year);
		current = current->next;
	}

	return 0;
}

#endif
