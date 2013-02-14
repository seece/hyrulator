#pragma once

#include <stdint.h>
#include "../romfile.hpp"

const uint32_t ram_size= 2048;
enum MemoryArea {MEM_UNKNOWN, MEM_WORKRAM, MEM_PPUCONTROL, MEM_MAPPER};

class Memory {

	public:
		Memory() ;  
		Memory(const RomFile* rom); 
		uint8_t readb(uint32_t offset);
		uint16_t readw(uint32_t offset);
		void write(uint32_t offset, uint8_t value);
	private:
		// includes zero page, stack and RAM
		uint8_t mRam[ram_size];
		const RomFile* rom;
};

MemoryArea getAreaFromOffset(uint32_t offset);
