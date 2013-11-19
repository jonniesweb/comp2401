
GCC = gcc


all:	a4.o a4Stack.o
	$(GCC) -o a4 a4.o a4Stack.o

a4.o:	a4.c a4Stack.c a4Defs.h a4Stack.h
	$(GCC) -c a4.c

a4Stack.o:	a4Stack.c a4Defs.h
	$(GCC) -c a4Stack.c

clean:
	rm -vf *.o a4
