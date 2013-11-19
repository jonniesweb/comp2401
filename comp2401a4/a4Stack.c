/*
 * a4Stack.c
 *
 *  Created on: Nov 14, 2013
 *      Author: jon
 */

//#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#include "a4Defs.h"

void freeNodeAndData(MovieNodeType *node) {
	free(node->data);
	free(node);
}

void addToRight(MovieNodeType *node, MovieNodeType *newNode) {
	newNode->next = node->next;
	newNode->prev = node;
	node->next->prev = newNode;
	node->next = newNode;
}

void addToLeft(MovieNodeType *node, MovieNodeType *newNode) {
	node->prev->next = newNode;
	newNode->prev = node->prev;
	node->prev = newNode;
	newNode->next = node;
}

void addToEmptyList(MovieNodeType** list, MovieNodeType* node) {
	*list = node;
	node->prev = NULL;
	node->next = NULL;
}

int add(MovieNodeType** list, MovieNodeType *node) {

	// If list is empty
	if (*list == NULL) {
		addToEmptyList(list, node);
		return OK;

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
					return OK;

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
					return OK;
				}
				current = current->next;
				continue;

			} else if (comparison < 0) {

				if (current->prev == NULL) {
					current->prev = node;
					node->next = current;
					node->prev = NULL;
					*list = node;
					return OK;

				} else {
					addToLeft(current, node);
					return OK;
				}
			}

		} while (1);
	}

	return NOK;
}

int removeByName(MovieNodeType **list, char *title) {

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

			return OK;

		} else {
			current = current->next;
		}
	}

	return NOK;
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
