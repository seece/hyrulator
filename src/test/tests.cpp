
// some simple tests for the emulator

#include <stdint.h>
#include <iostream>
#include "../main.hpp"
#include "../emulator.hpp"

int main(int argc, char *argv[]) {

	char * rompath = "rom/Zelda.NES";

	Emulator emu;
	RomFile rom = emu.loadRom(rompath);

	emu.freeRom(&rom);

	return 0;
}
