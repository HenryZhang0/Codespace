CC=g++
CFLAGS=-c -Wall

all: asm

asm: asm.o scanner.o
	$(CC) asm.o scanner.o -o asm

asm.o: asm.cc scanner.h constants.h
	$(CC) $(CFLAGS) asm.cc

scanner.o: scanner.cc scanner.h
	$(CC) $(CFLAGS) scanner.cc

clean:
	rm -rf *o asm
