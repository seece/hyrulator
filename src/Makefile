CC=g++
CFLAGS=-c -g -Wall -pedantic -O2
LDFLAGS=-lstdc++ -lSDL -lSDL_ttf
VPATH=src 
SOURCES=$(wildcard *.cpp) $(wildcard NES/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
# build the executable to the root dir
EXECUTABLE=../hyrulator

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS) 

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

.PHONY : clean
clean: 
	rm $(OBJECTS)
	rm $(EXECUTABLE)

