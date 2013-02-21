// the main emulator class
#include <stdint.h>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <ios>
#include "emulator.hpp"
#include "romfile.hpp"
#include "nintendo.hpp"
#include "SDL/SDL.h"

using std::cout;
using std::endl;

Emulator::Emulator() {
	cout << "Emulator booting up." << endl ;
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		cout << "Couldn't init SDL!" << endl;
		exit(1);
	}

	m_romLoaded = false;
	m_screenWidth = 600;
	m_screenHeight = 480;
	m_screen = SDL_SetVideoMode(m_screenWidth, m_screenHeight, 32, SDL_SWSURFACE);
	SDL_WM_SetCaption( "hyrulator", NULL );
}

Emulator::~Emulator() {
	SDL_Quit();
}

int32_t Emulator::updateCPU() {
	if (!m_romLoaded) {
		return -1;
	}

	return m_Nes.runCycle();
}

int32_t Emulator::run() {

	updateCPU();

	SDL_Event event;
	bool quit=false;

	while(quit==false) {
		while(SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				quit = true;
			}
		}
	}

	return 0;
}

void Emulator::loadRom(char* path) {
	m_rom = loadRomFile(path);
	m_Nes = NES(&m_rom);
	m_romLoaded = true;
}
