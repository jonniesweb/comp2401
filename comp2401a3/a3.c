/*
 * a3.c
 *
 *  Created on: Nov 2, 2013
 *      Author: Jon Simpson
 *
 *  This program asks the user to input up to 100 integers to then find the sum
 *  of it all either via recursion or iteration. The function call stack is
 *  implemented as a linked list. The function call stack is written to a file
 *  named "a3output.txt" (lame .txt extension), then exits.
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "a3Defs.h"

// Initialise global variable for writing to a file
FILE *outputFile;


/**
 * Helper function to initialise the function call stack
 *
 * @param stack The stack to initialise
 */
void initStack(StackType** stack) {
	*stack = (StackType *) malloc(sizeof(StackType));
	(*stack)->head = 0;
}
/**
 * Adds a new frame in LIFO order to the call stack.
 *
 * @param stack The stack to add the frame to
 * @param frame The frame to add to the stack
 */
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

/**
 * Frees the dynamically allocated memory for the specified node
 *
 * @param node The node to deallocate
 */
void freeNode(FrameNodeType* node) {
	free(node->data);
	free(node);
}

/**
 * Recursive helper function to free all Frame Nodes on the function call stack.
 * Passing the first node in the stack clears the entire stack.
 *
 * @param node The node to start deallocating nodes at
 */
void freeAllNodes(FrameNodeType *node) {
	if (node != NULL) {
		freeAllNodes(node->next);
		freeNode(node);
	}
}

/**
 * Removes from the stack the frame that was last added (LIFO)
 *
 * @param stack The stack to remove the frame from
 */
void pop(StackType *stack) {

	FrameNodeType *node;
	node = stack->head;

	stack->head = node->next;

	freeNode(node);
}

/**
 * Deallocate all the memory for the stack
 * @param stack The stack to deallocate
 */
void cleanupStack(StackType *stack) {

	freeAllNodes(stack->head);

	free(stack);
}

/**
 * Helper method to assist initialising VarTypes
 *
 * @param vartype
 * @param name
 * @param dType
 * @param value
 */
void initVarType(VarType *vartype, char *name, DataType dType, int *value) {

	strcpy(vartype->name, name);
	vartype->dType = dType;
	vartype->value = (int) value;
}

/**
 * Creates a new stack frame, fills the frame with data, pushes it onto the
 * stack, and outputs the stack to a3output.txt
 *
 * @param stack The stack to add a frame to
 * @param fname The name of the calling function
 * @param num The first parameter
 * @param arr The second parameter
 * @param sum The third parameter
 */
void enterSumFunc(StackType *stack, char *fname, int num, int *arr, int *sum) {

	// create the frame
	FrameType *frame;
	frame = (FrameType *) calloc(1, sizeof(FrameType));

	frame->funcName = fname;
	frame->numParms = MAX_PARMS;

	initVarType(&frame->parms[0], "num", C_INT, (int *) num);
	initVarType(&frame->parms[1], "arr", C_INT_PTR, arr);
	initVarType(&frame->parms[2], "sum", C_INT_PTR, sum);

	// add to the stack
	push(stack, frame);

	// print the contents
	dumpStack(stack);

}
/**
 * Outputs the contents of the stack then pops the top frame off of the stack
 * @param stack The stack to remove the frame from
 */
void leaveSumFunc(StackType *stack) {

	dumpStack(stack);
	pop(stack);
}

int main(void) {

	// declare function call stack
	StackType *stack;
	initStack(&stack);

	// open the file to write to
	outputFile = fopen("a3output.txt", "w");

	if (!outputFile) {
		printf("Error opening file\n");
		exit(ERR_OPENING_FILE);
	}

	/*
	 * Get a bunch of numbers from the user
	 */

	// allocate
	int *numArray;
	numArray = (int *) calloc(MAX_ARR_SIZE, sizeof(int));
	int input, returnVal, counter = 0;

	system("clear");
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
				"Enter 'i' for iterative addition or 'r' for recursive addition (i/r):\n");

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
	 * Get ready to exit the program by cleaning the stack, freeing memory and
	 * closing opened files
	 */
	cleanupStack(stack);
	free(numArray);

	// close outputFile
	fclose(outputFile);

	return C_OK;
}
