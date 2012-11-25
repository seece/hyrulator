// the main emulator class
#include <stdint.h>
#include <iostream>
#include "emulator.hpp"

Emulator::Emulator() {
	std::cout << "Emulator booting up." 
		<< std::endl;
}

int32_t Emulator::run() {
	std::cout << "Emulator would run a main loop here." 
		<< std::endl;

	return 0;
}
