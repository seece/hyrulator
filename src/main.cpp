// hyrulator

#include <stdint.h>
#include <iostream>
#include "main.hpp"
#include "emulator.hpp"

int main(int argc, char *argv[]) {

	// we could probably do some command line parameter parsing here
	// all the interesting stuff is delegated to the emulator class

	Emulator emu;
	emu.run();


	return 0;
}
