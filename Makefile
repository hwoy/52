CC=gcc
CXX=g++
BIN=52

STD=c++11

CXXFLAGS=-g --std=$(STD) -pedantic -Wall

.PHONY: all clean

all: $(BIN)

clean:
	del *.o *.exe $(BIN)

$(BIN): main.o
	$(CXX) -o $(BIN) main.o

main.o: main.cpp card.hpp player.hpp 52config.hpp 52.hpp 52io.hpp \
 52type.hpp



