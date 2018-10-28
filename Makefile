CXX=g++
CFLAGS=-std=c++14

OBJ=main math

all:build

build:$(OBJ)
	$(CXX) $(CFLAGS) -o calc.app obj/*.o

$(OBJ):
	$(CXX) -c $(CFLAGS) -o obj/$@.o $@.cpp

.PHONY:all build $(OBJ)
