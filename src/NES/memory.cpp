
#include <stdint.h>
#include <iostream>
#include <cstring>
#include "memory.hpp"

Memory::Memory() {
	// in the NES the memory is not zeroed and this might mask some bugs
	memset(mRam, 0, ram_size);
}

uint8_t Memory::readb(uint32_t offset) {
	// TODO add here some mapper processing
	return mRam[offset];
}

uint16_t Memory::readw(uint32_t offset) {
	// TODO add here some mapper processing
	return mRam[offset];
}

void Memory::write(uint32_t offset, uint8_t value) {
	// TODO add here some mapper processing too
	mRam[offset] = value;
}

