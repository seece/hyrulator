#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cstring>
#include "memory.hpp"
#include "../romfile.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::hex;
using std::setfill;
using std::setw;

Memory::Memory() {
	rom = NULL;
}

Memory::Memory(const RomFile* newrom) {
	// in the NES the memory is not zeroed and this might mask some bugs
	memset(mRam, 0, ram_size);
	rom = newrom;	
}

uint8_t Memory::mapMemory(uint32_t offset) {
	if (offset >= 0x8000) {
		// zero mapper in action
		uint32_t prg_offset = offset - 0x8000;
		return rom->romData[prg_offset];
	}

	return 0;
}

uint8_t Memory::readb(uint32_t offset) {
	MemoryArea area = getAreaFromOffset(offset);
	switch (area) {
		case MEM_UNKNOWN:
			// don't write to unknown positions
			fprintf(stdout, "Memory read from MEM_UNKNOWN at 0x%x\n", offset);
			break;
		case MEM_WORKRAM:
			cout << "Read from MEM_WORKRAM at 0x" << setw(2) << setfill('0')
			<< offset;
			//cout << ": " << hex << mRam[offset] << endl;
			fprintf(stdout, ": %0.X\n", mRam[offset]);
			return mRam[offset];
			break;
		case MEM_PPUCONTROL:
			fprintf(stdout, "PPU Control read requested from 0x%x\n", offset);
			break;
		case MEM_MAPPER:
			fprintf(stdout, "Mapper memory read from 0x%x\n", offset);
			return mapMemory(offset);

			break;
		default:
			fprintf(stderr, "Memory area error (read) at 0x%x\n", offset);
			break;
	}

	return 0;
}

uint16_t Memory::readw(uint32_t offset) {
	// TODO add mapper here
	// TODO put proper read logic here since now the two bytes can be fetched from totally different places
	char word[2];
	word[0] = readb(offset);
	word[1] = readb(offset+1);

	cout << "Read word " 
	<< hex << setw(2) << setfill('0') << word[0] //<< word[1]
	<< " from " << hex << offset << endl;
	return static_cast<uint16_t>(word[1] | (word[0] << 8));
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
	} else if (offset >= 0x2000 && offset <0x4000) {
		return MEM_PPUCONTROL;
	} else if (offset >= 0x4018 && offset < (0xC000+0x4000)) {
		return MEM_MAPPER;
	}

	return MEM_UNKNOWN;
}
