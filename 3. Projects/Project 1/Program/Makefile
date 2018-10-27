CC = g++
DEBUG = -lmenu -lncurses -g -O0 -pedantic-errors
CFLAGS = -Wall -std=c++11 -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all: p1

p1: p1.o
	$(CC) $(LFLAGS) -o p1 p1.o

p1.o: p1.cpp
	$(CC) $(CFLAGS) p1.cpp

.PHONY: clean

clean:
	rm -rf *.o
	rm -rf p1
