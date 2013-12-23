CA65=ca65
LD65=ld65
CFG=--cfg-path ./nes.cfg

SOURCES=$(wildcard src/*.s)
OBJECTS=$(SOURCES:.s=.o)
ROMS=$(SOURCES:.s=.nes)
# save to ../rom/
ROMS:=$(addprefix ../rom/,$(notdir $(ROMS)))

all: $(SOURCES) $(OBJECTS)

#PHONY: all
#all: ; $(info $$ROMS is [${ROMS}])

$(ROMS): $(OBJECTS)
	ld65 -o $@ -t nes $(OBJECTS) $(CFG)
	
.s.o:
	echo $(ROMS)
	ca65 $<

.PHONY : clean
clean:
	$(RM) $(OBJECTS)
#	$(RM) $(ROMS)
