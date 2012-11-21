CC=g++
CFLAGS=-c -Wall
LDFLAGS=
VPATH=src 
SOURCES=$(wildcard src/*.cpp) $(wildcard src/NES/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hyrulator

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

