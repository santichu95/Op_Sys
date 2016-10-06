CC=gcc
LDFLAGS= -lm -lpthread
CFLAGS=-Wall -pedantic -std=gnu99 

all: Prog0 Prog1

Prog0: triangle.o
	$(CC) $(LDFLAGS) triangle.o -o Prog0

triangle.o: triangle.c

Prog1: trithread.o
	$(CC) $(LDFLAGS) trithread.o -o Prog1

trithread.o: trithread.c

clean:
	rm *.o Prog1 Prog0

