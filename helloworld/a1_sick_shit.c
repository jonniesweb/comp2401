/******************************************************************************
 *
 * a1.c
 *
 * Takes an array of encrypted chars and decrypts each one, using the output as
 * an initialization vector for the next char to be decrypted. The IV and KEY
 * constants are used as the first initialization vector and in the encrypt()
 * function, respectively. Outputs the decrypted chars to stdout.
 *
 *  Created on: Sep 26, 2013
 *  Author: Jonathan Simpson
 *
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// include constants from a1Defs.h
#ifndef MAX_BUF
#define MAX_BUF 4096
#endif

#ifndef IV
#define IV 109
#endif

#ifndef KEY
#define KEY 0x73
#endif

/**
 * Gets the bit at place n of byte c
 *
 * @param c get bit from this char
 * @param n get bit at n'th position. 0 being least significant bit
 * @return either a 1 or 0
 */
unsigned char getBit(unsigned char c, int n) {

	return (c & (1 << n)) >> n;

}

/**
 * Set the bit to 1 at position n of byte c
 *
 * @param c char to switch bit of
 * @param n bit to set at n'th position. 0 being least significant bit
 * @return the char with the bit set
 */
unsigned char setBit(unsigned char c, int n) {

	return c | (1 << n);

}

/**
 * Set the bit to 0 at position n of byte c
 *
 * @param c the char that contains the bit to clear
 * @param n bit to clear at n'th position. 0 being least significant bit
 * @return c with the n'th bit set to 0
 */
unsigned char clearBit(unsigned char c, int n) {

	return c & (~(1 << n));

}

/**
 * Perform a circular left shift of all the bits in byte c by 1
 *
 * @param c the byte to shift
 * @return the shifted bits of byte c
 */
unsigned char rotl(unsigned char c) {

	unsigned char y = getBit(c, 7);
	unsigned char x = c << 1;

	return x ^ y;
}

/**
 * Perform a circular right shift of all the bits in byte c by 1
 *
 * @param c the byte to shift
 * @return the shifted bits of byte c
 */
unsigned char rotr(unsigned char c) {

	unsigned char y = getBit(c, 0) << 7;
	unsigned char x = c >> 1;

	return x ^ y;

}

/**
 * Inverts the n'th bit of c. Helper method for swapBits()
 *
 * @param c the byte to invert a bit from
 * @param n the bit to invert at the n'th position. 0 being the least significant bit
 * @return c with the inverted n'th bit
 */
unsigned char invertBit(unsigned char c, int n) {

	return c ^ (1 << n);
}

/**
 * Swap the values of pos1 and pos2 with each other of byte c
 *
 * @param c the byte to switch bits
 * @param pos1 the bit to switch with pos2
 * @param pos2 the bit to switch with pos1
 * @return the byte c with the bits pos1 and pos2 swapped with each other
 */
unsigned char swapBits(unsigned char c, int pos1, int pos2) {

	unsigned char x = getBit(c, pos1);
	unsigned char y = getBit(c, pos2);

	if (x == y) { // if the bits are the same, don't swap
		return c;

	} else { // if the bits are different, invert each
		c = invertBit(c, pos1);
		c = invertBit(c, pos2);
		return c;
	}

}

/**
 * start with the value of the c as the basis for the resulting byte
 * examine each bit of the key, starting at the most significant bit
 * - if the current bit of the key has value 0, swap the following two bits of
 *   the resulting byte: the bit in the current bit position, and the bit two
 *   positions to the left (circling back to the least significant bits, if necessary)
 * - if the current bit of the key has value 1, perform a circular right shift
 *   on the resulting byte
 *
 * @param c the byte to modify
 * @param key the decryption key
 * @return the decrypted byte
 */
unsigned char encrypt(unsigned char c, unsigned char key) {

	for (int i = 7; i >= 0; i--) {

		if (getBit(key, i) == 0) { // if the bit is 0, swap bits

			// explicitly show each usage case
			if (i == 7) {
				c = swapBits(c, 7, 1);

			} else if (i == 6) {
				c = swapBits(c, 6, 0);

			} else {
				c = swapBits(c, i, i + 2);
			}

		} else { // if the bit is 1, rotate right
			c = rotr(c);
		}
	}
	return c;
}

/**
 * The algorithm works as follows. At every iteration i:
 * 1. encrypt the value of IVi using the key
 * 2. exclusive-or (XOR) the value of Ci with the encrypted IVi to compute the value of Pi
 * 3. perform a circular left shift on the value of the encrypted IVi to compute the value of IVi+1
 *
 * @return exit code. 0 if normal. 1 if empty ciphertext
 */
int main(int argc, unsigned char *argv[]) {

	// array of ciphertext: text to be decrypted. Included from a1Defs.h
	unsigned char cipher[] = { 62, 138, 241, 129, 8, 221, 163, 159, 91, 158,
			169, 150, 15, 138, 251, 198, 14, 211, 161, 158, 77, 204, 188, 217,
			28, 216, 253, 137, 20, 202, 171, 128, 74, 133, 180, 209, 78, 155,
			255, 128, 28, 217, 167, 156, 91, 204, 179, 216, 26, 151, 190, 133,
			21, 216, 171, 220 };

	// create a pointer to the array of ciphers
	unsigned char *cipherText = &(cipher[0]);

	{
		// if we have the correct number of command line arguments
		if (argc == 4) {
			cipherText = argv[3];

			puts(cipherText);

			/* Determine the number of elements passed via command line args and
			 * store the resulting number in numElements. Start numElements with
			 * an initial value of 1 to offset counting n-1 commas.
			 */
			int numElements = 1;
			{
				int i = 0;
				while (cipherText[i] != '\0') {

					if (cipherText[i] == ',') {
						numElements++;
					}
					i++;
				}
				printf("%d\n", numElements);
			}

			// allocate some memory to put the pointers in
			char **a = (char **) calloc(numElements, sizeof(char));

			/* Iterate over all the elements of cipherText, initially setting
			 * the first element of a[0] to cipherText[0]. When the inner while
			 * loop reaches a comma character, replace it with a NULL character,
			 * then increment the a counter. Do this until it fills a up.
			 */
			{
				int ccount = 0;
				int jcount = 0;
				a[jcount] = &(cipherText[ccount]);

				while (cipherText[ccount] != '\0') { // go till end of cipherText
					if (cipherText[ccount] == ',') { // if comma encountered replace comma with NULL and set a to the pointer right after the NULL
						cipherText[ccount] = '\0';

						// increment
						jcount++;
						ccount++;

						// and assign
						a[jcount] = &(cipherText[ccount]);

					} else {
						// if not a comma, then just increment
						ccount++;

					}
					printf("c: %d j: %d CT: %c \n", ccount, jcount,
							cipherText[ccount]);

				}
			}
			for (int j = 0; j < numElements; ++j) {
				printf("%s\n", a[j]);
			}
		}

	}

// error checking to quit the program if the ciphertext is empty
	if (sizeof(cipherText) == 0) {
		printf("The ciphertext is empty. Quitting program.");
		exit(1);
	}

	int cipherTextSize = strlen(cipherText);
	unsigned char clearText[cipherTextSize];
	unsigned char iv = IV;

// iterate through the array of ciphertext
	for (int i = 0; i < cipherTextSize; ++i) {
		iv = encrypt(iv, KEY);

		clearText[i] = cipherText[i] ^ iv;
		iv = rotl(iv);
	}

// output the cleartext to stdout
	for (int i = 0; i < cipherTextSize; i++) {
//		printf("%c", clearText[i]);
	}
	printf("\n");
}

