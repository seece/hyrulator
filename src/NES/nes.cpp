

#include <stdint.h>
#include <cstdio>
#include <iostream>
#include "nes.hpp"

NES::NES(const RomFile* rom) {
	cpu = CPU(rom);

}

void NES::restart() {

}

