CC=clang
CFLAGS = -Wall -Wextra -g3

all: array.o assignment.o common.o conditions.o functions.o parameters.o registers.o variables.o bpl.o
	$(CC) array.o assignment.o common.o conditions.o functions.o parameters.o registers.o variables.o bpl.o -o bpl.exe $(CFLAGS)

array.o:
	$(CC) lib/array.c -o $(@) -c $(CFLAGS)

assignment.o:
	$(CC) lib/assignment.c -o $(@) -c $(CFLAGS)

common.o:
	$(CC) lib/common.c -o $(@) -c $(CFLAGS)

conditions.o:
	$(CC) lib/conditions.c -o $(@) -c $(CFLAGS)

functions.o:
	$(CC) lib/functions.c -o $(@) -c $(CFLAGS)

parameters.o:
	$(CC) lib/parameters.c -o $(@) -c $(CFLAGS)

registers.o:
	$(CC) lib/registers.c -o $(@) -c $(CFLAGS)

variables.o:
	$(CC) lib/variables.c -o $(@) -c $(CFLAGS)

bpl.o:
	$(CC) bpl.c -o $(@) -c $(CFLAGS)

clear:
	rm -rf *.o