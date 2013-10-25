#define MAX_FRAMES   100
#define MAX_PARMS      3
#define MAX_STR       32
#define MAX_BYTES      4
#define MAX_ARR_SIZE 100
#define C_OK           0
#define C_NOK         -1

typedef enum {C_INT, C_SHORT, C_CHAR, C_FLOAT, C_INT_PTR} DataType;

/*  Stack related data types  */

typedef struct VarType {
	char name[MAX_STR];
	int value;
	DataType dType;

} VarType;

typedef struct FrameType {
	char funcName[MAX_STR];
	int numParms;
	VarType parms[MAX_PARMS];

} FrameType;

typedef struct StackType {
	int numFrames;
	FrameType frames[MAX_FRAMES];

} StackType;

/*  Stack related functions  */
void dumpStack(StackType*);
void dumpVar(VarType*);
void dumpBytes(char*, int);
void convertToBytes(int num, unsigned char *bytes);
int getNumBytes(DataType);

/*  Bit related functions  */
int           getIntBit(int, int);
unsigned char setBit(unsigned char, int);
unsigned char clearBit(unsigned char, int);

/*  Loop functions  */
void sumIterative(StackType*, int, int*, int*);
void sumRecursive(StackType*, int, int*, int*);

/*  Instrumentation functions  */
void enterSumFunc(StackType*, char*, int, int*, int*);
void leaveSumFunc(StackType*);
