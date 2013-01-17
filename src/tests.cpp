
// some simple tests for the emulator

#include <stdint.h>
#include <iostream>
#include "main.hpp"
#include "emulator.hpp"

int main(int argc, char *argv[]) {

	char * rompath = "rom/Zelda.NES";
	Emulator emu;
	emu.loadRom(rompath);

	return 0;
}
