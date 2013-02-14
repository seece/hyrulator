#pragma once

#include <stdint.h>
#include "NES/nes.hpp"

class Emulator {
	public:
		Emulator();

		int32_t run(void);
	private:
		NES * m_pNes; // TODO figure this out later
};
