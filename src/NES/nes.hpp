	//NES
#pragma once
#include <stdint.h>
#include "cpu.hpp"
#include "memory.hpp"
#include "ppu.hpp"
#include "../romfile.hpp"

class NES {
	public:
		NES(const RomFile* rom);
		void restart();
		int32_t runCycle(void);
		
	private:
		CPU cpu;
		PPU ppu;
		//APU apu;
};
