#pragma once

#include <stdint.h>
#include "NES/nes.hpp"

class Emulator {
	public:
		Emulator();
		int32_t run();
	private:
		NES mNes;
};
