// hyrulator

#include <stdint.h>
#include <iostream>
#include "main.hpp"
#include "emulator.hpp"

int main(int argc, char *argv[]) {

	// we could probably do some command line parameter parsing here
	// all the interesting stuff is delegated to the emulator class

	char * rompath = "rom/Zelda.NES";

	RomFile rom = loadRom(rompath);
	Emulator emu;
	emu.run();

	freeRom(&rom);

	return 0;
}
