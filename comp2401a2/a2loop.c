/*
 * a2loop.c
 *
 *  Created on: Oct 18, 2013
 *      Author: Jonathan Simpson
 *
 *  This file contains the iterative or recursive functions of adding an array
 *  of numbers.
 */

#include <stdio.h>
#include <string.h>

#include "a2Defs.h"

/*   Function:  sumIterative
 *     in/out:  stack used to track values
 *         in:  number of elements in int array
 *         in:  int array values
 *        out:  sum of ints in int array
 *    Purpose:  compute sum iteratively
 */

void sumIterative(StackType *stkPtr, int numElements, int *intArray, int *sum) {

	enterSumFunc(stkPtr, "sumIterative", numElements, intArray, sum);

	int i;
	*sum = 0;

	for (i=0; i<numElements; ++i)
		*sum += intArray[i];

	leaveSumFunc(stkPtr);
}

/*   Function:  sumRecursive
 *     in/out:  stack used to track values
 *         in:  number of elements in int array
 *         in:  int array values
 *        out:  sum of ints in int array
 *    Purpose:  compute sum recursively
 */

void sumRecursive(StackType *stkPtr, int numElements, int *intArray, int *sum) {

	enterSumFunc(stkPtr, "sumRecursive", numElements, intArray, sum);

	if (numElements == 0) {
		*sum = 0;
//		sumRecursive(stkPtr, numElements-1, intArray, sum);
		leaveSumFunc(stkPtr);
		return;
	}

	sumRecursive(stkPtr, numElements-1, intArray, sum);
	*sum += intArray[numElements-1];

	leaveSumFunc(stkPtr);
}
