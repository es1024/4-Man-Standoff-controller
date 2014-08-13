CC = gcc
CXX = g++

CFLAGS = -Wall -pthread -ggdb
CPPFLAGS = -std=c++11

all: build/standoff.out

build/standoff.out: build/standoff.o build/entry.o build/main.o build/popen_noshell.o
	$(CXX) $(CFLAGS) -o $@ $^
	
build/standoff.o: src/standoff.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c -o $@ $^
build/entry.o: src/entry.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c -o $@ $^
build/main.o: src/main.cpp
	$(CXX) $(CFLAGS) $(CPPFLAGS) -c -o $@ $^
build/popen_noshell.o: src/popen_noshell.c
	$(CC) $(CFLAGS) -c -o $@ $^
clean:
	rm -rf build/*.out build/*.o


