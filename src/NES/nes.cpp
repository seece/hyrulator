

#include <stdint.h>
#include <cstdio>
#include <iostream>
#include "nes.hpp"

NES::NES(const RomFile* rom) {
	cpu = CPU(rom);
	cpu.resetInterrupt();

}

void NES::restart() {

}

int32_t NES::runCycle(void) {
	cpu.changeState();
	return cpu.getClock();
}
