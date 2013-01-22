#pragma once

#include <stdint.h>
#include "NES/nes.hpp"

class Emulator {
	public:
		Emulator();

		static RomFile loadRom(char * filepath);
		int32_t run(void);
	private:
		NES m_Nes;
};
