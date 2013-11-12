#define MAX_PARMS      3
#define MAX_STR       32
#define MAX_BYTES      4

#define MAX_ARR_SIZE 100
#define TRUE		   1

// Return values
#define C_OK           0
#define C_NOK         -1
#define ERR_VARTYPE_PARAMETER_OUT_OF_BOUNDS 1
#define ERR_OPENING_FILE 2

// Define global variable for file output
extern FILE *outputFile;

/*  Stack related data types  */

typedef enum {C_INT, C_SHORT, C_CHAR, C_FLOAT, C_INT_PTR} DataType;

typedef struct {
  char     name[MAX_STR]; // changed since Christine mentioned that its alright to do this
  DataType dType;
  int      value;
} VarType;

typedef struct {
  char    *funcName;
  int     frameNum;
  int     numParms;
  VarType parms[MAX_PARMS];
} FrameType;


/*	Singly-Linked-List data types	*/

typedef struct FrameNodeType FrameNodeType;

typedef struct FrameNodeType {
	FrameNodeType *next;
	FrameType *data;
} FrameNodeType;

typedef struct {
	FrameNodeType *head;
} StackType;



/*  Stack management functions  */
void initStack(StackType**);
void push(StackType*, FrameType*);
void pop(StackType*);
void cleanupStack(StackType*);

/*  Stack utility functions  */
void dumpStack(StackType*);
void dumpVar(VarType*);
void dumpBytes(char*, int);
void convertToBytes(int num, unsigned char *bytes);
int  getNumBytes(DataType);

/*  Bit related functions  */
int  getIntBit(int, int);
unsigned char setBit(unsigned char, int);
unsigned char clearBit(unsigned char, int);

/*  Loop functions  */
void sumIterative(StackType*, int, int*, int*);
void sumRecursive(StackType*, int, int*, int*);

/*  Instrumentation functions  */
void enterSumFunc(StackType*, char*, int, int*, int*);
void leaveSumFunc(StackType*);
