CC=clang
CFLAGS = -Wall -Wextra -g3

all: bin/array.o bin/assignment.o bin/common.o bin/conditions.o bin/functions.o bin/parameters.o bin/registers.o bin/variables.o bin/bpl.o
	$(CC) bin/array.o bin/assignment.o bin/common.o bin/conditions.o bin/functions.o bin/parameters.o bin/registers.o bin/variables.o bin/bpl.o -o bpl.exe -no-pie
	cp bpl.exe tests/compiler

bin/array.o: lib/array.c
	$(CC) lib/array.c -o $(@) -c $(CFLAGS)

bin/assignment.o: lib/assignment.c
	$(CC) lib/assignment.c -o $(@) -c $(CFLAGS)

bin/common.o: lib/common.c
	$(CC) lib/common.c -o $(@) -c $(CFLAGS)

bin/conditions.o: lib/conditions.c
	$(CC) lib/conditions.c -o $(@) -c $(CFLAGS)

bin/functions.o: lib/functions.c
	$(CC) lib/functions.c -o $(@) -c $(CFLAGS)

bin/parameters.o: lib/parameters.c
	$(CC) lib/parameters.c -o $(@) -c $(CFLAGS)

bin/registers.o: lib/registers.c
	$(CC) lib/registers.c -o $(@) -c $(CFLAGS)

bin/variables.o: lib/variables.c
	$(CC) lib/variables.c -o $(@) -c $(CFLAGS)

bin/bpl.o: bpl.c
	$(CC) bpl.c -o $(@) -c $(CFLAGS)

clear:
	rm -rf *.o
	rm -rf *.exe
	rm -rf bin/*.o
	rm -rf bin/*.exe

run:
	./bpl.exe tests/functions/t1.bpl output.s