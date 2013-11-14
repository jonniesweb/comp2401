/*
 * a4Stack.c
 *
 *  Created on: Nov 14, 2013
 *      Author: jon
 */

#include <stdlib.h>
#include <string.h>
#include "a4Defs.h"

void addToTail(MovieList* list, MovieNodeType* node) {
	// add to the end
	list->tail->next = node;
	node->prev = list->tail;
	node->next = NULL;
	list->tail = node;
}

void addToHead(MovieList *list, MovieNodeType *node) {
	node->next = list->head;
	node->prev = NULL;
	list->head->prev = node;
	list->head = node;
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

void addToEmptyList(MovieList* list, MovieNodeType* node) {
	list->head = node;
	list->tail = node;
	node->prev = NULL;
	node->next = NULL;
}

int add(MovieList *list, MovieNodeType *node) {

	// If list is empty
	if (list->head == NULL && list->tail == NULL) {
		addToEmptyList(list, node);
		return OK;

	} else {
		MovieNodeType *current = list->head;
		do {
			int comparison = strcmp(node->data->title, current->data->title);
			if (comparison > 0) {
				// advance to the next node

				if (current->next == NULL) {
					// add to the end
					addToTail(list, node);
					return OK;
				} else {
					current = current->next;
					continue;
				}

				// if the titles are equal
			} else if (comparison == 0) {
//				addToRight(current, node);
//				return OK; // TODO remove when below is implemented
				// compare the years

				if (current->data->year > node->data->year) { // XXX will fuck up probably
					if (current->prev == NULL) {
						addToHead(list, node);
					} else {
						addToLeft(current, node);
					}
					return OK;
				}
				current = current->next;
				continue;
			} else if (comparison < 0) {
				if (current->prev == NULL) {
					addToHead(list, node);
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

void initNode(MovieNodeType** node, char *title, GenreType genre, int year) {
	*node = (MovieNodeType*) malloc(sizeof(MovieNodeType));
	(*node)->data = (MovieType*) malloc(sizeof(MovieType));
	(*node)->data->genre = genre;
	strcpy((*node)->data->title, title);
	(*node)->data->year = year;
}

//int findByTitle(MovieList *list, char *title) {
//
//}

#if 1
#include <stdio.h>

int main(void) {

	MovieList *list = (MovieList *) malloc(sizeof(MovieList));

	list->head = NULL;
	list->tail = NULL;

	MovieNodeType *a;
	MovieNodeType *b;
	MovieNodeType *c;

	initNode(&a, "a", C_ACTION, 1994);
	initNode(&b, "a", C_HORROR, 1984);
	initNode(&c, "a", C_SF, 2013);

	add(list, c);
	add(list, b);
	add(list, a);


	/*
	 * Output list
	 */
	MovieNodeType *current = list->head;
	while (current != NULL) {
		printf("%s : %d\n", current->data->title, current->data->year);
		current = current->next;
	}

	return 0;
}


#endif
