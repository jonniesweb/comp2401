/*
 * a3loop.c
 *
 *  Created on: Nov 5, 2013
 *      Author: Jon Simpson
 *
 *  This file contains the iterative or recursive functions of adding an array
 *  of numbers.
 */

#include <stdio.h>
#include <string.h>

#include "a3Defs.h"

/*   Function:  sumIterative
 *     in/out:  stack used to track values
 *         in:  number of elements in int array
 *         in:  int array values
 *        out:  sum of ints in int array
 *    Purpose:  compute sum iteratively
 */

void sumIterative(StackType *stack, int numElements, int *intArray, int *sum) {

	enterSumFunc(stack, "sumIterative", numElements, intArray, sum);

	int i;
	*sum = 0;

	for (i=0; i<numElements; ++i)
		*sum += intArray[i];

	leaveSumFunc(stack);
}

/*   Function:  sumRecursive
 *     in/out:  stack used to track values
 *         in:  number of elements in int array
 *         in:  int array values
 *        out:  sum of ints in int array
 *    Purpose:  compute sum recursively
 */

void sumRecursive(StackType *stack, int numElements, int *intArray, int *sum) {

	enterSumFunc(stack, "sumRecursive", numElements, intArray, sum);

	if (numElements == 0) {
		*sum = 0;
		leaveSumFunc(stack);
		return;
	}

	sumRecursive(stack, numElements-1, intArray, sum);
	*sum += intArray[numElements-1];

	leaveSumFunc(stack);
}

