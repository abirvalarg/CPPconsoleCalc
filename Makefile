CXX=g++ -std=c++14
CFLAGS=-c
LFLAGS=

SRC=$(wildcard *.cpp)
HDR=$(wildcard *.hpp)
ifeq ($(OS),Windows_NT)
OBJ=$(SRC:%.cpp=obj\%.o)
else
OBJ=$(SRC:%.cpp=obj/%.o)
endif
OUTPUT=calc

all: $(OUTPUT)
	@mkdir obj

$(OUTPUT): $(OBJ)
	$(CXX) $(LFLAGS) -o $@ $^

obj/%.o: %.cpp $(HDR)
	$(CXX) $(CFLAGS) -o $@ $<

ifeq ($(OS),Windows_NT)
clear:
	@del /s obj $(OUTPUT)
else
clear:
	@rm -r obj $(OUTPUT) 2> /dev/null || true
endif

.PHONY: all clear
