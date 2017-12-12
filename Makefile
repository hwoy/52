CC=gcc
CXX=g++

D=d
R=r

BIND=52-$(D)
BINR=52-$(R)

STD=c++11

DEBUG=-g

RELEASE=-O2

CXXFLAGS=--std=$(STD) -pedantic -Wall

.PHONY: all clean

all: $(BIND) $(BINR)

clean:
	del *.o *.exe $(BIND) $(BINR) 

$(BIND): main-$(D).o
	$(CXX) -o $(BIND) main-$(D).o
	
$(BINR): main-$(R).o
	$(CXX) -o $(BINR) main-$(R).o


main-$(D).o: main.cpp card.hpp player.hpp 52config.hpp 52.hpp 52io.hpp \
 52type.hpp Copt.hpp computer.hpp human.hpp
	$(CXX) -c $(DEBUG) -o main-$(D).o main.cpp
	
main-$(R).o: main.cpp card.hpp player.hpp 52config.hpp 52.hpp 52io.hpp \
 52type.hpp Copt.hpp computer.hpp human.hpp
	$(CXX) -c $(RELEASE) -o main-$(R).o main.cpp
