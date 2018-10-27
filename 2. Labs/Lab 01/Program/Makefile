CC = g++
DEBUG = -g
CFLAGS = -Wall -std=c++11 -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all: lab01

lab01: lab01.o
	$(CC) $(LFLAGS) -o lab01 lab01.o

lab01.o: lab01.cpp
	$(CC) $(CFLAGS) lab01.cpp

.PHONY: clean

clean:
	rm -rf *.o
	rm -rf lab01
