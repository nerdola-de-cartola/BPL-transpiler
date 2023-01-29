CC=clang
CFLAGS = -Wall -Wextra -g3

all: array.o assignment.o common.o conditions.o functions.o parameters.o registers.o variables.o bpl.o
	$(CC) array.o assignment.o common.o conditions.o functions.o parameters.o registers.o variables.o bpl.o -o bpl.exe
	cp bpl.exe tests/compiler

array.o: lib/array.c
	$(CC) lib/array.c -o $(@) -c $(CFLAGS)

assignment.o: lib/assignment.c
	$(CC) lib/assignment.c -o $(@) -c $(CFLAGS)

common.o: lib/common.c
	$(CC) lib/common.c -o $(@) -c $(CFLAGS)

conditions.o: lib/conditions.c
	$(CC) lib/conditions.c -o $(@) -c $(CFLAGS)

functions.o: lib/functions.c
	$(CC) lib/functions.c -o $(@) -c $(CFLAGS)

parameters.o: lib/parameters.c
	$(CC) lib/parameters.c -o $(@) -c $(CFLAGS)

registers.o: lib/registers.c
	$(CC) lib/registers.c -o $(@) -c $(CFLAGS)

variables.o: lib/variables.c
	$(CC) lib/variables.c -o $(@) -c $(CFLAGS)

bpl.o: bpl.c
	$(CC) bpl.c -o $(@) -c $(CFLAGS)

clear:
	rm -rf *.o

run:
	./bpl.exe tests/conditions/t1.bpl output.s