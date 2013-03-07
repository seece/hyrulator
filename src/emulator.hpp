#pragma once

#include <stdint.h>
#include "NES/nes.hpp"
#include "debugger.hpp"
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
		void drawText(int32_t x, int32_t y, const char * message);
		void render();
		bool update();

		NES m_Nes; 
		RomFile m_rom;
		bool m_romLoaded;

		Debugger m_debugger;

		SDL_Surface* m_screen;
		SDL_Color m_bgColor;
		TTF_Font* m_font;
		int32_t m_screenWidth;
		int32_t m_screenHeight;

};


