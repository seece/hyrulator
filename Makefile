CC=g++
CFLAGS=-c -Wall
LDFLAGS=
VPATH=src 
SOURCES=NES/cpu.cpp NES/nes.cpp NES/ppu.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=hyrulator

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

