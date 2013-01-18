CC=g++
CFLAGS=-c -g -Wall -pedantic -O2
LDFLAGS=-lstdc++
VPATH=src 
SOURCES=$(wildcard *.cpp)
ALL_SOURCES=$(wildcard ../*.cpp) $(wildcard ../NES/*.cpp)
ALL_SOURCES:=$(filter-out ../main.cpp, $(ALL_SOURCES))
TEST_OBJECTS=$(SOURCES:.cpp=.o)
OBJECTS=$(TEST_OBJECTS) $(ALL_SOURCES:.cpp=.o)
# build the executable to the root dir
EXECUTABLE=../../tests

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY : clean
clean: 
	rm $(EXECUTABLE)
	rm $(TEST_OBJECTS)

