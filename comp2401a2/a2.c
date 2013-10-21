/*
 * a2.c
 *
 *  Created on: Oct 18, 2013
 *      Author: jon
 */

#include "a2Defs.h";
#include <stdio.h>;

int main(int argc, char **argv) {

	// define function callstack variable
	StackType stack;

	/*
	 * Get a bunch of numbers from the user
	 */

	int numArray[10]; // TODO change value
	int input, returnVal, counter = 0;

	puts("Enter some numbers");

	while (1) {

		returnVal = scanf("%d", &input);

		if (returnVal == 1) {
			numArray[counter] = input;
			counter++;

		} else { // if the return value is anything other than what we want
			break;
		}

	}

	/*
	 * Prompt the user to use either iterative or recursive adding
	 */
	char sumMethod;
	printf("%s", "Enter 'i' for iterative addition or 'r' for recursive addition (i/r): ");


	while ((sumMethod = getchar()) != '\n' && sumMethod != EOF);

	while (1) {
//		printf("%d		%d\n", scanf("%c", &sumMethod), sumMethod);

		scanf("%c", &sumMethod);

		if (sumMethod == 'i') {
			puts("i works");
			break;

		} else if (sumMethod == 'r') {
			puts("r works");
			break;

		} else {
			continue;
		}
	}

}

