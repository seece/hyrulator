#pragma once

#include <stdint.h>

const uint32_t ram_size= 2048;

class Memory {

	public:
		Memory(); // placeholder, should take arguments
		uint8_t read(uint32_t offset);
		void write(uint32_t offset, uint8_t value);
	private:
		// includes zero page, stack and RAM
		uint8_t mRam[ram_size];
};

