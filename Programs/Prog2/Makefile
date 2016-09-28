OBJS=buffet.o driver.o
CC=g++
CFLAGS= -std=c++11 -Wall -Werror -pedantic -Wextra
LFLAGS=

buffet: driver.o buffet.o 
	$(CC) $(LFLAGS) $(OBJS) -o buffet

driver.o: driver.cpp
	$(CC) -c $(CFLAGS) driver.cpp

buffet.o: buffet.h buffet.cpp
	$(CC) -c $(CFLAGS) buffet.cpp



clean: 
	rm *o buffet
