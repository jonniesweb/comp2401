/*
 * a2.c
 *
 *  Created on: Oct 18, 2013
 *      Author: Jonathan Simpson
 *
 *  This program asks the user to input up to 100 integers to then find the sum
 *  of it all either via recursion or iteration. The callstack information is
 *  displayed before the resulting sum is shown.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a2Defs.h"

int main(void) {

	// define function callstack variable
	StackType stack;
	stack.numFrames = 0;

	/*
	 * Get a bunch of numbers from the user
	 */

	// allocate
	int *numArray;
	numArray = (int*) calloc(MAX_ARR_SIZE, sizeof(int));
	int input, returnVal, counter = 0;

	puts(
			"Enter a number on each newline. Up to 100 integers can be entered.\nEnter 'r' or 'i' for recursive or iterative addition (i/r)");

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
	printf(
			"Enter 'i' for iterative addition or 'r' for recursive addition (i/r): ");

	while (1) {

		scanf("%c", &sumMethod); // get char from stdin

		if (sumMethod == 'i') {
			sumIterative(&stack, counter, numArray, &sum);
			break;

		} else if (sumMethod == 'r') {
			sumRecursive(&stack, counter, numArray, &sum);
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
	 * Cleanup
	 */
	free(numArray);

	return C_OK;

}

/**
 * Push a stackframe onto the stack
 *
 * @param stkPtr The stack
 * @param fname Name of function
 * @param num length of arr
 * @param arr array of numbers
 * @param sum the current sum of the recursive function
 */
void enterSumFunc(StackType *stkPtr, char *fname, int num, int *arr, int *sum) {

	// set frame values
	stkPtr->frames[stkPtr->numFrames].numParms = 3;
	strcpy(stkPtr->frames[stkPtr->numFrames].funcName, fname);

	// set VarType for num
	strcpy(stkPtr->frames[stkPtr->numFrames].parms[0].name, "num");
	stkPtr->frames[stkPtr->numFrames].parms[0].value = num;
	stkPtr->frames[stkPtr->numFrames].parms[0].dType = C_INT;

	// set VarType for arr
	strcpy(stkPtr->frames[stkPtr->numFrames].parms[1].name, "arr");
	stkPtr->frames[stkPtr->numFrames].parms[1].value = (int) arr;
	stkPtr->frames[stkPtr->numFrames].parms[1].dType = C_INT_PTR;

	// set VarType for sum
	strcpy(stkPtr->frames[stkPtr->numFrames].parms[2].name, "sum");
	stkPtr->frames[stkPtr->numFrames].parms[2].value = (int) sum;
	stkPtr->frames[stkPtr->numFrames].parms[2].dType = C_INT_PTR;

	stkPtr->numFrames++;

}

/**
 * Prints out the stack and then pops a stack frame from the stack
 * @param stkPtr The stack to operate on
 */
void leaveSumFunc(StackType *stkPtr) {

	// dump the stack to stdout
	dumpStack(stkPtr);

	// decrement the number of frames on the stack
	// the frame is "removed"
	stkPtr->numFrames--;

}
