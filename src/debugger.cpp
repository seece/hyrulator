#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include "debugger.hpp"
#include "NES/nes.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::setfill;
using std::setw;
using std::hex;
using std::string;

const int columnSize = 8;

// replaces newlines and NULs with spacebars
#define REPLACE_UNPRINTABLE(c) ((c==0x00) ? (0x20) : ((c==0x0A) ? 0x20 : c))

Debugger::Debugger() {
	nes = NULL;
}

void Debugger::attachNes(NES* nes_pointer) {
	nes = nes_pointer;
}

void Debugger::printRomDump(uint32_t begin, uint32_t end) {
	if (nes==NULL) {
		cerr << "Debugger not initialized!" << endl;
		return;
	}

	cout << "ROM dump" << endl;

	std::string hexText;

	int32_t columns = 0;
	for (uint32_t i=begin;i<end;i++) {
		uint8_t byte = nes->cpu.mMem.rom->romData[i];

		if (columns == 0) {
			cout << setw(4) << hex << (begin+i) << "\t";
		}

		char chartext[2];
		chartext[0] = REPLACE_UNPRINTABLE(byte);
		chartext[1] = '\0';
		hexText.append(chartext);

		cout << hex << setw(2) << setfill('0') << int(byte) << " ";

		columns++;

		if (columns == columnSize) {
			columns = 0;
			cout << hexText << endl;
			hexText.clear();
		}
	}

	cout << endl;
}
