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
#include "SDL/SDL_ttf.h"

using std::cout;
using std::cerr;
using std::endl;

Emulator::Emulator() {
	cout << "Emulator booting up." << endl ;
	if (SDL_Init(SDL_INIT_EVERYTHING)) {
		cerr << "Couldn't init SDL!" << endl;
		exit(1);
	}

	if (TTF_Init() == -1) {
		cerr << "Couldn't init SDL_ttf!" << endl;
		exit(1);
	}

	m_font = TTF_OpenFont("data/FreeMono.ttf", 12);

	if (m_font == NULL) {
		cerr << "Couldn't load font!" << endl;
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

	bool quit=false;

	while(quit==false) {
		quit = update();
	}

	return 0;
}

void Emulator::loadRom(char* path) {
	m_rom = loadRomFile(path);
	m_Nes = NES(&m_rom);
	m_romLoaded = true;
}

// return true to quit
bool Emulator::update() {
	SDL_Event event;

	while(SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return true;
		}
	}

	this->render();

	return false;
}

void Emulator::render() {
	SDL_Rect screensize= {0, 0, m_screen->w, m_screen->h};
	Uint32 bgColor = SDL_MapRGB(m_screen->format, 25,25,30);
	SDL_FillRect(m_screen, &screensize, bgColor);
	this->renderDebugView();
	if (SDL_Flip(m_screen)) {
		cerr << "SDL error!" << endl;
		exit(1);
	}
}

void Emulator::renderDebugView() {
	SDL_Color white = {255, 255, 255};
	SDL_Surface * text = TTF_RenderText_Solid(m_font, "this is a test", white);

	SDL_Rect rect = {10, 10, text->w, text->h};
	SDL_BlitSurface(text, NULL, m_screen, &rect);
	SDL_FreeSurface(text);
}

