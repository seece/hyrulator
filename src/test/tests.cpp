
// some simple tests for the emulator

#include <stdint.h>
#include <iostream>
#include "../main.hpp"
#include "../romfile.hpp"
#include "../emulator.hpp"
#include "../debugger.hpp"
#include "../nintendo.hpp"

int main(int argc, char *argv[]) {

	char * rompath = "rom/Zelda.NES";

	Emulator emu;
	Debugger debug;
	RomFile rom = loadRomFile(rompath);

	NES nes(&rom);
	debug.attachNes(&nes);
	//debug.printRomDump(0, ROM_BANKSIZE);
	debug.printRomDump(0, 128);

	for (int32_t i=0;i<10;i++) {
		int32_t cycle = nes.runCycle();
	}

	freeRomFile(&rom);

	return 0;
}
