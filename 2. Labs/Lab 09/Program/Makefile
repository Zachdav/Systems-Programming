CC = g++
DEBUG = -lncurses -g -O0 -pedantic-errors
CFLAGS = -Wall -std=c++11 -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

all: chmod

chmod: chmod.o
	$(CC) $(LFLAGS) -o chmod chmod.o

chmod.o: chmod.cpp
	$(CC) $(CFLAGS) chmod.cpp

.PHONY: clean

clean:
	rm -rf *.o
	rm -rf chmod
