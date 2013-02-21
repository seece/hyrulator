#pragma once

#include <stdint.h>
#include "NES/nes.hpp"
#include "SDL/SDL.h"

class Emulator {
	public:
		Emulator();
		~Emulator();

		int32_t run(void);
	private:
		NES * m_pNes; // TODO figure this out later
		SDL_Surface* m_screen;
		int32_t m_screenWidth;
		int32_t m_screenHeight;
};
