OBJS=buffet.o driver.o
CC=g++
CFLAGS= -std=c++11 -Wall -Werror -pedantic -Wextra
LFLAGS= -lpthread
JC = javac
.SUFFIXES: .java .class
.java.class:
	javac $*.java
CLASSES = \
		  TestDriver.java \
		  SliceType.java \
		  Buffet.java \
		  BuffetMonitor.java \
		  BuffetSemaphore.java
all: classes buffet

classes: $(CLASSES:.java=.class)


buffet: driver.o buffet.o 
	$(CC) $(LFLAGS) $(OBJS) -o buffet

buffet.o: buffet.h buffet.cpp
	$(CC) -c $(CFLAGS) buffet.cpp

driver.o: driver.cpp
	$(CC) -c $(CFLAGS) driver.cpp

clean: 
	$(RM) *o buffet *.class
