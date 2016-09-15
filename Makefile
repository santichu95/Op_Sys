C=gcc
LDFLAGS= -lm -lpthread
CFLAGS=-Wall -Werror -pedantic -std=gnu99 
SOURCES=trithread.c
OBJECTS:=$(SOURCES:.c=.o)

thread: trithread.o
	$(CC) $(LDFLAGS) trithread.o -o thread

trithread.o: trithread.c

clean:
	rm *.o thread
