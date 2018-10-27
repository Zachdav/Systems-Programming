CC = g++
DEBUG = -lncurses -g -O0 -pedantic-errors
CFLAGS = -Wall -std=c++11 -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all: lab06test lab06

lab06test: lab06test.o
	$(CC) $(LFLAGS) -o lab06test lab06test.o

lab06test.o: lab06test.cpp
	$(CC) $(CFLAGS) lab06test.cpp

lab06: lab06.o
	$(CC) $(LFLAGS) -o lab06 lab06.o

lab06.o: lab06.cpp
	$(CC) $(CFLAGS) lab06.cpp

.PHONY: clean

clean:
	rm -rf *.o
	rm -rf lab06test
	rm -rf lab06
