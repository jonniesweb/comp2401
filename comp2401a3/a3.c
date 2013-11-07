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
	*stack = (StackType *) malloc(sizeof(StackType));
	(*stack)->head = 0;
}

void push(StackType *stack, FrameType *frame) {

	FrameNodeType *node;
	node = (FrameNodeType *) calloc(1, sizeof(FrameNodeType));

	// if the stack is empty
	if (stack->head == NULL) {

		/*
		 * Dr. Laurendeau said that starting at either 0 or 1 is fine
		 */
		frame->frameNum = 0;

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
	int i;
	for (i = 0; i < node->data->numParms; ++i) {
		free(node->data->parms[i].name);
	}
	free(node->data);
	free(node);
}

/**
 * Recursive helper function to free all Frame Nodes on the function call stack
 * @param node
 */
void freeAllNodes(FrameNodeType *node) {
	if (node != NULL) {
		freeAllNodes(node->next);
		freeNode(node);
	}
}

void pop(StackType *stack) {

	FrameNodeType *node;
	node = stack->head;

	stack->head = node->next;

	freeNode(node);
}

/**
 * Deallocate all the memory for the stack
 * @param stack
 */
void cleanupStack(StackType *stack) {

	freeAllNodes(stack->head);

	free(stack);
}

void initVarType(FrameType *frame, int varPosition, char *name, DataType dType,
		int *value) {

	// error check to see if the varPosition variable will be out of bounds
	if (varPosition < 0 || varPosition >= frame->numParms) {
		exit(ERR_VARTYPE_PARAMETER_OUT_OF_BOUNDS);
	}

	frame->parms[varPosition].name = (char *) calloc(MAX_STR, sizeof(char));
	strcpy(frame->parms[varPosition].name, name);
	frame->parms[varPosition].dType = dType;
	frame->parms[varPosition].value = (int) value;
}

/**
 * Creates a new stack frame, fills the frame with data, pushes it onto the
 * stack, and dumps the stack to stdout.
 * @param stack
 * @param fname
 * @param num
 * @param arr
 * @param sum
 */
void enterSumFunc(StackType *stack, char *fname, int num, int *arr, int *sum) {

	// create the frame
	FrameType *frame;
	frame = (FrameType *) calloc(1, sizeof(FrameType));

	frame->funcName = fname;
	frame->numParms = MAX_PARMS;

	initVarType(frame, 0, "num", C_INT, num);
	initVarType(frame, 1, "arr", C_INT_PTR, (int) arr);
	initVarType(frame, 2, "sum", C_INT_PTR, (int) sum);

	// add to the stack
	push(stack, frame);

	// print the contents
	dumpStack(stack);

}

void leaveSumFunc(StackType *stack) {

	// output stack to stdout
	dumpStack(stack);

	// pop off the last frame from the stack
	pop(stack);

}

int main(void) {

	// declare function call stack
	StackType *stack;
	initStack(&stack);

	/*
	 * Get a bunch of numbers from the user
	 */

	// allocate
	int *numArray;
	numArray = (int *) calloc(MAX_ARR_SIZE, sizeof(int));
	int input, returnVal, counter = 0;

	printf(
			"Enter a number on each newline. Up to 100 integers can be entered.\nEnter 'r' or 'i' for recursive or iterative addition (i/r)\n");

	while (counter < MAX_ARR_SIZE) {

		returnVal = scanf("%d", &input);

		if (returnVal == 1) {
			numArray[counter] = input;
			counter++;

		} else { // if the return value is anything other than what we want
			break;
		}

	}

	/*
	 * Prompt the user to use either iterative or recursive adding, then use the
	 * respective method.
	 */
	char sumMethod;
	int sum = 0;

	while (TRUE) {
		printf(
				"Enter 'i' for iterative addition or 'r' for recursive addition (i/r): ");

		scanf("%c", &sumMethod); // get char from stdin

		if (sumMethod == 'i') {
			sumIterative(stack, counter, numArray, &sum);
			break;

		} else if (sumMethod == 'r') {
			sumRecursive(stack, counter, numArray, &sum);
			break;

		} else {
			continue;
		}
	}

	/*
	 * Output the answer
	 */
	printf("The sum of the numbers is %d\n", sum);

	/*
	 * Get ready to exit the program by cleaning the stack
	 */
	cleanupStack(stack);
	free(numArray);

	return C_OK;
}
