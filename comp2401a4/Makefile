
GCC = gcc


all:	a4.o a4List.o
	$(GCC) -o a4 a4.o a4List.o

a4.o:	a4.c a4List.c a4Defs.h
	$(GCC) -c a4.c

a4List.o:	a4List.c a4Defs.h
	$(GCC) -c a4List.c

clean:
	rm -vf *.o a4 a4output.txt
