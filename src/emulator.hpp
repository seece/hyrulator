	#pragma once

#include <stdint.h>
#include "NES/nes.hpp"

class Emulator {
	public:
		Emulator();
		int32_t run();
		int32_t loadRom(char * filepath);
	private:
		NES mNes;
};
