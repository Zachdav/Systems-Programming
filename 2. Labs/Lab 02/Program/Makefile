CC=g++
DEBUG=-g
CFLAGS=-Wall -std=c++11 -c $(DEBUG)
LFLAGS=-Wall $(DEBUG)

all: stat minmax degrees time endtime

stat: stat.o
	$(CC) $(LFLAGS) -o stat stat.o

stat.o: stat.cpp
	$(CC) $(CFLAGS) stat.cpp

minmax: minmax.o
	$(CC) $(LFLAGS) -o minmax minmax.o

minmax.o: minmax.cpp
	$(CC) $(CFLAGS) minmax.cpp

degrees: degrees.o
	$(CC) $(LFLAGS) -o degrees degrees.o

degrees.o: degrees.cpp
	$(CC) $(CFLAGS) degrees.cpp

time: time.o
	$(CC) $(LFLAGS) -o time time.o

time.o: time.cpp
	$(CC) $(CFLAGS) time.cpp

endtime: endtime.o
	$(CC) $(LFLAGS) -o endtime endtime.o

endtime.o: endtime.cpp
	$(CC) $(CFLAGS) endtime.cpp

.PHONY: clean

clean:
	rm -rf *.o
	rm -rf stat
	rm -rf minmax
	rm -rf degrees
	rm -rf time
	rm -rf endtime
