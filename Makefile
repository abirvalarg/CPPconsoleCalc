CXX=g++ -std=c++14
CFLAGS=-c
LFLAGS=

SRC=$(wildcard *.cpp)
HDR=$(wildcard *.hpp)
OBJ=$(SRC:%.cpp=obj/%.o)
OUTPUT=calc

ifeq ($(OS),Windows_NT)
	MKDIR=rem
else
	MKDIR=mkdir -p
endif

ifeq ($(OS),Windows_NT)
all: $(OUTPUT)
	@mkdir obj
else
all: $(OUTPUT)
endif

$(OUTPUT): $(OBJ)
	@$(MKDIR) $(@D)
	$(CXX) $(LFLAGS) -o $@ $^

obj/%.o: %.cpp $(HDR)
	@$(MKDIR) $(@D)
	$(CXX) $(CFLAGS) -o $@ $<

ifeq ($(OS),Windows_NT)
clear:
	@del /s obj $(OUTPUT)
else
clear:
	@rm -r obj $(OUTPUT) 2> /dev/null || true
endif

.PHONY: all clear
