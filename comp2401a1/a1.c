/*
 * a1.c
 *
 *  Created on: Sep 26, 2013
 *      Author: jon
 */


#include <stdio.h>

#ifndef MAX_BUF
#define MAX_BUF 4096
#endif

#ifndef IV
#define IV 109
#endif

#ifndef KEY
#define KEY 0x73
#endif


unsigned char getBit(unsigned char c, int n) {

  return (c & (1 << n)) >> n;

}
unsigned char setBit(unsigned char c, int n) {

  return c | (1 << n);

}
unsigned char clearBit(unsigned char c, int n) {

  return c & (~ (1 << n));

}

unsigned char rotl(unsigned char c) {

  unsigned char x = c << 1;
  unsigned char y = getBit(c, 7);

  return x ^ y;
}

unsigned char rotr(unsigned char c) {

  unsigned char x = c >> 1;
  unsigned char y = getBit(c, 0) << 7;

  return x ^ y;

}

unsigned char invertBit(unsigned char c, int pos) {

  return c ^ (1 << pos);
}

unsigned char swapBits(unsigned char c, int pos1, int pos2) {

  unsigned char x = getBit(c, pos1);
  unsigned char y = getBit(c, pos2);

  if (x == y)
  {
    return c;

  } else {
    unsigned char z = invertBit(c, pos1);
    z = invertBit(c, pos2);
    return z;
  }

}


void toBinary(unsigned int c) {

  int output = 0;

  output = setBit(getBit(c, 0), 0);

  printf("%d\n", output);
}

unsigned char encrypt(unsigned char c, unsigned char key) {

  for (int i = 7; i >= 0; i--) {
    if (getBit(c, i) == 0) {
      if (i == 7) {
        c = swapBits(c, 7, 1);

      } else if (i == 6) {
        c = swapBits(c, 6, 0);

      } else {
        c = swapBits(c, i, i+2);

      }
    } else { // getBit() == 1
      c = rotr(c);
    }
  }
  return c;
}


int mainz() {

  // printf("%c\n", rotl(rotr('A')));

  // unsigned char c = 'b';
  // printf("%d\n", c);
  // unsigned char z = invertBit(c, 6);
  // printf("%d\n", c);
  // printf("%d\n", z);

  // unsigned char c = 'k';
  // printf("%d\n", c);
  // c = swapBits(c, 0, 1);
  // printf("%d\n", c);

  unsigned char cipherText[] = {62,138,241,129,8,221,163,159,91,158,169,150,15,
  138,251,198,14,211,161,158,77,204,188,217,28,216,253,137,20,202,171,128,74,
  133,180,209,78,155,255,128,28,217,167,156,91,204,179,216,26,151,190,133,21,
  216,171,220};

  int cipherTextSize = sizeof(cipherText)/sizeof(char);
  unsigned char clearText[cipherTextSize];
  unsigned char iv = IV;


  for (int i = 0; i < cipherTextSize; ++i)
  {
    printf("%d\n", i);
    iv = encrypt(iv, KEY);
    clearText[i] = cipherText[i] ^ iv;

    iv = rotl(iv);
  }

  for (int i = 0; i < cipherTextSize; i++)
  {
    printf("%c", clearText[i]);
  }
  printf("\n");
}
