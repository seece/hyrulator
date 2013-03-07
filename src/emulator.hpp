#pragma once

#include <stdint.h>
#include "NES/nes.hpp"
#include "nintendo.hpp"
#include "romfile.hpp"
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

class Emulator {
	public:
		Emulator();
		~Emulator();

		int32_t run(void);
		void loadRom(char* path);
		int32_t updateCPU();
	private:
		void renderDebugView();
		void render();
		bool update();
		NES m_Nes; // TODO figure this out later
		SDL_Surface* m_screen;
		SDL_Color m_bgColor;
		TTF_Font* m_font;
		bool m_romLoaded;
		int32_t m_screenWidth;
		int32_t m_screenHeight;
		RomFile m_rom;

		
};
