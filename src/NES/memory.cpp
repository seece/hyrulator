
#include <stdint.h>
#include <iostream>
#include <cstring>
#include "memory.hpp"
#include "../romfile.hpp"

Memory::Memory() {
}

Memory::Memory(const RomFile& newrom) {
	// in the NES the memory is not zeroed and this might mask some bugs
	memset(mRam, 0, ram_size);
	//rom = &newrom;	
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

// based on http://en.wikibooks.org/wiki/NES_Programming/Memory_Map
MemoryArea getAreaFromOffset(uint32_t offset) {
	if (offset < 0x800) {
		return MEM_WORKRAM;
	} else if (offset < 0x1800) {
		return MEM_WORKRAM; // mirrored
	} else if (offset > 0x2000 && offset <0x4000) {
		return MEM_PPUCONTROL;
	} else if (offset > 0x4020 && offset < (0xC000+0x4000)) {
		return MEM_MAPPER;
	}

	return MEM_UNKNOWN;
}
