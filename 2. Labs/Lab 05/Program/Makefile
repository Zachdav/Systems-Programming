CC = g++
DEBUG = -g -O0 -pedantic-errors
CFLAGS = -Wall -std=c++11 -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all: lab04

lab04: lab04.o Matrix.o
	$(CC) $(LFLAGS) -o lab04 lab04.o Matrix.o

lab04.o: lab04.cpp Matrix.h
	$(CC) $(CFLAGS) lab04.cpp

Matrix.o: Matrix.cpp Matrix.h
	$(CC) $(CFLAGS) Matrix.cpp

.PHONY: clean

clean:
	rm -rf *.o
	rm -rf lab04
