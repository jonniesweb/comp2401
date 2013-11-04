/*
 * a3.c
 *
 *  Created on: Nov 2, 2013
 *      Author: jon
 */

#include "a3Defs.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * Helper function to initialise the function call stack
 *
 * @param stack
 */
void initStack(StackType** stack) {
	*stack = (StackType*) calloc(1, sizeof(StackType));
	(*stack)->head = 0; // TODO check if this is right!
}

void push(StackType *stack, FrameType *frame) {

	FrameNodeType *node;
	node = (FrameNodeType *) calloc(1, sizeof(FrameNodeType));


	// if the stack is empty
	if (stack->head == 0) {

		frame->frameNum = 1; // TODO ask if this is correct

		node->data = frame;
		node->next = 0;

		stack->head = node;

		// if the stack is not empty
	} else {
		frame->frameNum = stack->head->data->frameNum + 1;

		node->data = frame;
		node->next = stack->head;

		stack->head = node;
	}
}

void freeNode(FrameNodeType* node) {
	free(node->data->funcName); // XXX may have to disable this
	free(node->data);
	free(node);
}

void pop (StackType *stack) {

	FrameNodeType *node;
	node = stack->head;

	stack->head = node->next;

	freeNode(node);
}

/**
 * Recursive helper function to free all Frame Nodes on the function call stack
 * @param node
 */
void freeAllNodes(FrameNodeType *node) {
	if (node->next != NULL) {
		freeAllNodes(node->next);
	}
	free(node->data->funcName); // XXX may have to disable this
	free(node->data);
	free(node);
}

/**
 * Deallocate all the memory for the stack
 * @param stack
 */
void cleanupStack(StackType *stack) {

	freeAllNodes(stack->head);

	free(stack);
}

int main(void) {

	// declare function call stack
	StackType *stack;
	initStack(&stack); // TODO check if this is right!

	FrameType *frame1;
	frame1 = calloc(1, sizeof(FrameType));
	frame1->funcName = calloc(20, sizeof(char));
	strcpy(frame1->funcName, "test 123");
	frame1->numParms = 3;
	frame1->parms[0].dType = C_INT;
	frame1->parms[0].value = 50;

	FrameType *frame2;
	frame2 = calloc(1, sizeof(FrameType));
	frame2->funcName = calloc(20, sizeof(char));
	strcpy(frame2->funcName, "456 test");
	frame2->numParms = 3;
	frame2->parms[0].dType = C_INT;
	frame2->parms[0].value = 50;

	push(stack, frame1);
	push(stack, frame2);

//	pop(stack);
//	pop(stack);

	cleanupStack(stack);

	printf("stuff works");

	return 0;
}
