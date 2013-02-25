// hyrulator

#include <stdint.h>
#include <iostream>
#include "main.hpp"
#include "romfile.hpp"
#include "emulator.hpp"

int main(int argc, char *argv[]) {

	// we could probably do some command line parameter parsing here
	// all the interesting stuff is delegated to the emulator class

	char * rompath = "rom/Zelda.NES";

	//RomFile rom = loadRomFile(rompath);
	Emulator emu;
	emu.loadRom(rompath);
	emu.run();

	return 0;
}
