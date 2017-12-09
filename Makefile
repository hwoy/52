CC=gcc
CXX=g++
BIN=52

STD=c++14

CXXFLAGS=--std=$(STD) -pedantic -Wall

.PHONY: all clean

all: $(BIN)

$(BIN): main.o
	$(CXX) -o $(BIN) main.o

main.o: main.cpp card.hpp player.hpp 52io.hpp

clean:
	del *.o *.exe $(BIN)

