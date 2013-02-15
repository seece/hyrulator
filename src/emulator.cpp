// the main emulator class
#include <stdint.h>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <ios>
#include "emulator.hpp"
#include "romfile.hpp"
#include "nintendo.h"

Emulator::Emulator() {
	std::cout << "Emulator booting up." ;

	// TODO add NES class initialization here
}

int32_t Emulator::run() {
	std::cout << "Emulator would run a main loop here." 
		<< std::endl;

	return 0;
}


