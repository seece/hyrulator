#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include "debugger.hpp"
#include "NES/nes.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::setfill;
using std::setw;
using std::hex;

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

	int32_t columns = 0;
	for (uint32_t i=begin;i<end;i++) {
		uint8_t byte = nes->cpu.mMem.rom->romData[i];

		if (columns == 0) {
			cout << setw(4) << hex << (begin+i) << "\t";
		}

		cout << hex << setw(2) << setfill('0') << int(byte) << " ";

		columns++;

		if (columns == 8) {
			columns = 0;
			cout << endl;
		}
	}

	cout << endl;
}
