OBJS=buffet.o
CC=g++
CFLAGS=-c -std=c++11 -Wall -Werror -pedantic -Wextra
LFLAGS=

buffet: buffet.o
	$(CC) $(LFLAGS) $(OBJS) -o buffet

buffet.o: buffet.cpp
	$(CC) $(CFLAGS) buffet.cpp

clean: *.o buffet
