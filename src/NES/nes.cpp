

#include <stdint.h>
#include <cstdio>
#include <iostream>
#include "nes.hpp"

NES::NES(const RomFile* rom) {
	cpu = CPU(rom);
	restart();
}

void NES::restart() {
	cpu.resetInterrupt();
}

int32_t NES::runCycle(void) {
	cpu.changeState();
	return cpu.getClock();
}
