CC = gcc
CFLAGS = -std=gnu99 -Wpedantic

all: simcpu

simcpu: simcpu.o parser.o thread.o heap.o
	$(CC) $(CFLAGS) simcpu.o parser.o thread.o heap.o -o simcpu

simcpu.o: simcpu.c
	$(CC) $(CFLAGS) -c simcpu.c -o simcpu.o

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c -o parser.o

thread.o: thread.c
	$(CC) $(CFLAGS) -c thread.c -o thread.o

heap.o: heap.c
	$(CC) $(CFLAGS) -c heap.c -o heap.o

clean:
	rm *o simcpu