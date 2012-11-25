
#include <stdint.h>
#include <iostream>
#include "memory.hpp"

Memory::Memory() {
	// in the NES the memory is not zeroed and this might mask some bugs
	memset(mRam, 0, MEM_SIZE);
}

uint8_t Memory::read(uint8_t offset) {
	// TODO add here some mapper processing
	return mRam[offset];
}

void Memory::write(uint8_t offset, uint8_t value) {
	// TODO add here some mapper processing too
	mRam[offset] = value;
}

