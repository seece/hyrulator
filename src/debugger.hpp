#pragma once
#include <stdint.h>
#include "NES/nes.hpp"

class Debugger {
	public:
		Debugger();	
		void attachNes(NES* nes_pointer);
		// prints ROM data
		void printRomDump(uint32_t begin, uint32_t end);
	private:
		NES* nes;
};
