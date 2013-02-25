	//NES
#pragma once
#include <stdint.h>
#include "cpu.hpp"
#include "memory.hpp"
#include "ppu.hpp"
#include "../romfile.hpp"

class NES {
	public:
		NES() {};
		NES(const RomFile* rom);
		void restart();
		int32_t runCycle(void);
		friend class Debugger;
		
	private:
		CPU cpu;
		PPU ppu;
		//APU apu;
};
