#pragma once
#include <stdint.h>
#include <string>
#include "NES/nes.hpp"
#include "NES/cpu.hpp"


class Debugger {
	public:
		Debugger();	
		void attachNes(NES* nes_pointer);
		// prints ROM data
		void printRomDump(uint32_t begin, uint32_t end);
		std::string getRegisterState();
	private:
		NES* nes;
};
