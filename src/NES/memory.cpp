
#include <stdint.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include "memory.hpp"
#include "../romfile.hpp"

Memory::Memory() {
}

Memory::Memory(const RomFile* newrom) {
	// in the NES the memory is not zeroed and this might mask some bugs
	memset(mRam, 0, ram_size);
	rom = newrom;	
}

uint8_t Memory::readb(uint32_t offset) {
	MemoryArea area = getAreaFromOffset(offset);
	switch (area) {
		case MEM_UNKNOWN:
			// don't write to unknown positions
			fprintf(stdout, "Memory read from MEM_UNKNOWN at 0x%x\n", offset);
			break;
		case MEM_WORKRAM:
			return mRam[offset];
			break;
		case MEM_PPUCONTROL:
			fprintf(stdout, "PPU Control read requested from 0x%x\n", offset);
			break;
		case MEM_MAPPER:
			fprintf(stdout, "Mapper memory read from 0x%x\n", offset);
			// TODO add here mapper processing
			break;
		default:
			fprintf(stderr, "Memory area error (read) at 0x%x\n", offset);
			break;
	}

	return 0;
}

uint16_t Memory::readw(uint32_t offset) {
	// TODO add here some mapper processing
	fprintf(stderr, "readw not yet supported, tried to read from 0x%x\n", offset);
	return mRam[offset];
}

void Memory::write(uint32_t offset, uint8_t value) {
	MemoryArea area = getAreaFromOffset(offset);
	switch (area) {
		case MEM_UNKNOWN:
			// don't write to unknown positions
			fprintf(stdout, "Memory write to MEM_UNKNOWN at 0x%x\n", offset);
			break;
		case MEM_WORKRAM:
			mRam[offset] = value;
			break;
		case MEM_PPUCONTROL:
			fprintf(stdout, "PPU Control write requested from 0x%x\n", offset);
			break;
		case MEM_MAPPER:
			fprintf(stdout, "Mapper memory write from 0x%x\n", offset);
			// TODO add here mapper processing
			break;
		default:
			fprintf(stderr, "Memory area error (write) at 0x%x\n", offset);
			break;
	}
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
