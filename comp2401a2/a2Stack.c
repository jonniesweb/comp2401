/*
 * a2Stack.c
 *
 *  Created on: Oct 18, 2013
 *      Author: jon
 */

#include <stdio.h>
#include <string.h>

#include "a2Defs.h"

/*   Function:  dumpStack
/*         in:  stack to be output
/*    Purpose:  output stack to screen        */

void dumpStack(StackType *stk)
{
  int i, j;

  printf("     ------------ STACK -----------\n");

  for (i=0; i<stk->numFrames; ++i) {
    printf("     ---------- FRAME #%d:  %s ----------\n", i, stk->frames[i].funcName);
    for (j=0; j<stk->frames[i].numParms; ++j) {
      printf("     -- Parm #%d:\n", j);
      dumpVar(&(stk->frames[i].parms[j]));
    }
  }
  printf("     -------- END OF STACK --------\n\n");
}

/*   Function:  dumpVar
/*         in:  variable to be output
/*    Purpose:  output variable to screen     */

void dumpVar(VarType *var)
{
  unsigned char bytes[MAX_BYTES];
  int nBytes;

  printf("     ----  name:  %s\n", var->name);

  convertToBytes(var->value, bytes);

  printf("     ---- value:  ");
  nBytes = getNumBytes(var->dType);
  dumpBytes(bytes, nBytes);

  if (var->dType == C_INT_PTR) {
    printf("     ----  ptee:  ");
    convertToBytes((int) *((int*)var->value), bytes);
    dumpBytes(bytes, MAX_BYTES);
  }
}

/*   Function:  dumpBytes
/*         in:  byte array to be output
/*         in:  number of bytes in byte array
/*    Purpose:  output byte array as hex      */

void dumpBytes(char *b, int n)
{
  int k;

  for (k=n-1; k>=0; --k) {
    printf("%02x ", (unsigned char)b[k]);
  }
  printf("\n");
}

/*   Function:  convertToBytes
/*         in:  int to be converted to byte array
/*        out:  resulting byte array
/*    Purpose:  separate int into 4 bytes          */

void convertToBytes(int num, unsigned char *bytes)
{
  int i, j;
  unsigned char tmpChar;

  for (i=0; i<MAX_BYTES; ++i) {
    tmpChar = 0;
    for (j=0; j<8; ++j) {
      if (getIntBit(num, j+(i*8)) == 0)
        tmpChar = clearBit(tmpChar, j);
      else
        tmpChar = setBit(tmpChar, j);
    }
    bytes[i] = tmpChar;
  }
}

/*   Function:  getNumBytes
/*         in:  data type whose num of bytes is returned
/*        ret:  size of data type
/*    Purpose:  return number of bytes in given type     */

int getNumBytes(DataType dt)
{
  switch(dt) {
//    case C_INT:     return 4;
//    case C_SHORT:   return 2;
//    case C_CHAR:    return 1;
//    case C_FLOAT:   return 4;
//    case C_INT_PTR: return 4;

  // give better values that work with both 32/64 bit systems
  case C_INT:     return sizeof(int);
  case C_SHORT:   return sizeof(short);
  case C_CHAR:    return 1;
  case C_FLOAT:   return sizeof(float);
  case C_INT_PTR: return sizeof(int);
  }

}

int getIntBit(int x, int n)
{
  return ((x & (1 << n)) >> n);
}

unsigned char setBit(unsigned char c, int n)
{
  return(c | (1 << n));
}

unsigned char clearBit(unsigned char c, int n)
{
  return (c & (~(1 << n)));
}

