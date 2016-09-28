OBJS=buffet.o driver.o
CC=g++
CFLAGS= -std=c++11 -Wall -Werror -pedantic -Wextra
LFLAGS= -lpthread

buffet: driver.o buffet.o 
	$(CC) $(LFLAGS) $(OBJS) -o buffet

buffet.o: buffet.h buffet.cpp
	$(CC) -c $(CFLAGS) buffet.cpp

driver.o: driver.cpp
	$(CC) -c $(CFLAGS) driver.cpp

clean: 
	rm *o buffet
