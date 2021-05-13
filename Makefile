CXX=g++ -std=c++14
CFLAGS=-c
LFLAGS=

SRC=$(wildcard *.cpp)
HDR=$(wildcard *.hpp)
OBJ=$(SRC:%.cpp=%.o)
OUTPUT=calc

all: $(OUTPUT)

$(OUTPUT): $(OBJ)
	$(CXX) $(LFLAGS) -o $@ $^

%.o: %.cpp $(HDR)
	$(CXX) $(CFLAGS) -o $@ $<

ifeq ($(OS),Windows_NT)
clear:
	@del /s *.o $(OUTPUT)
else
clear:
	@rm *.o $(OUTPUT) 2> /dev/null || true
endif

.PHONY: all clear
