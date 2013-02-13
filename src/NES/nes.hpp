//NES
#pragma once
#include "cpu.hpp"
#include "memory.hpp"
#include "ppu.hpp"
#include "../romfile.hpp"

class NES {
	public:
		NES(const RomFile &rom);
		void restart();
		
	private:
		CPU cpu;
		PPU ppu;
		//APU apu;
};
