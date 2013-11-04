#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a3Defs.h"

/*   Function:  dumpStack                     */
/*         in:  stack to be output            */
/*    Purpose:  output stack to screen        */

void dumpStack(StackType *stk)
{

  printf("     ------------ STACK -----------\n");


  printf("     -------- END OF STACK --------\n\n");
}

/*   Function:  dumpVar                       */
/*         in:  variable to be output         */
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

/*   Function:  dumpBytes                     */
/*         in:  byte array to be output       */
/*         in:  number of bytes in byte array */
/*    Purpose:  output byte array as hex      */

void dumpBytes(char *b, int n)
{
  int k;

  for (k=n-1; k>=0; --k) {
    printf("%02x ", (unsigned char)b[k]);
  }
  printf("\n");
}

/*   Function:  convertToBytes                     */
/*         in:  int to be converted to byte array  */
/*        out:  resulting byte array               */
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

/*   Function:  getNumBytes                              */
/*         in:  data type whose num of bytes is returned */
/*        ret:  size of data type                        */
/*    Purpose:  return number of bytes in given type     */

int getNumBytes(DataType dt)
{
  switch(dt) {
    case C_INT:     return 4;
    case C_SHORT:   return 2;
    case C_CHAR:    return 1;
    case C_FLOAT:   return 4;
    case C_INT_PTR: return 4;
    default:        return 4;
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

void printChar(char x)
{
  int i;

  for (i=7; i>=0; i--) {
    if (((x & (1 << i)) >> i) == 0)
      printf("0");
    else
      printf("1");
    if (i%4 == 0)
      printf(" ");
  }
}

void printInt(int x)
{
  int i;

  for (i=31; i>=0; i--) {
    if (((x & (1 << i)) >> i) == 0)
      printf("0");
    else
      printf("1");
    if (i%4 == 0)
      printf(" ");
  }
}
