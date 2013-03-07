#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstring>
#include "debugger.hpp"
#include "NES/nes.hpp"
#include "NES/cpu.hpp"

using std::cout;
using std::cerr;
using std::endl;
using std::setfill;
using std::setw;
using std::hex;
using std::string;
using std::stringstream;

const int columnSize = 8;

// replaces newlines and NULs with spacebars
#define REPLACE_UNPRINTABLE(c) ((c==0x00) ? (0x20) : ((c==0x0A) ? 0x20 : c))

Debugger::Debugger() {
	nes = NULL;
}

void Debugger::attachNes(NES* nes_pointer) {
	nes = nes_pointer;
}

std::string Debugger::getRegisterState() {
	Registers reg = nes->cpu.mReg;
	stringstream stream;
	stream << setfill(' ') << hex;
	stream << "A: " 	<< setw(2) << static_cast<int32_t>(reg.A);
	stream << " X: " 	<< setw(2) << static_cast<int32_t>(reg.X);
	stream << " Y: " 	<< setw(2) << static_cast<int32_t>(reg.Y);
	stream << " S: " 	<< setw(2) << static_cast<int32_t>(reg.S);
	stream << " SP: " 	<< setw(2) << static_cast<int32_t>(reg.SP);
	stream << " PC: " 	<< setw(4) << static_cast<int32_t>(reg.PC);
	string s = stream.str();

	return s;
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
