CXX = g++

CFLAGS = -Wall -std=c++11 -pthread -O3


all: build/standoff.out

build/standoff.out: src/standoff.cpp src/entry.cpp src/main.cpp
	$(CXX) $(CFLAGS) -o $@ $^

clean:
	rm -rf build/*.out


