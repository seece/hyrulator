#include <stdint.h>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <ios>
#include "romfile.hpp"
#include "nintendo.hpp"

#define BITFLAG(flags,num) (((flags & (1<<num)) == 1) ? true : false)

static void readFlags(RomFile * romp, uint8_t flags1, uint8_t flags2) {
	romp->mirrorDirection= BITFLAG(flags1, 0); 
	romp->SRAMEnabled= BITFLAG(flags1, 1); 
	romp->trainerPresent= BITFLAG(flags1, 2); 
	romp->fourScreenLayout= BITFLAG(flags1, 3); 
	romp->vsSystem = BITFLAG(flags2, 0); 

	uint8_t mapper = (flags1 & 0x0F) | ((flags2 & 0x0F)<<4);
	romp->mapper = (int32_t) mapper;

}

void freeRomFile(RomFile* romp) {
	delete romp->romData;	
	delete romp->vromData;	
}

// Loads a rom to the emulator memory.
// returns NULL on failure
// based on http://fms.komkon.org/EMUL8/NES.html#LABM
RomFile loadRomFile(char * filepath) {
	RomFile rom;

	std::ifstream input (filepath, std::ifstream::in);
	uint32_t ident;
	uint8_t numRomBanks;
	uint8_t numVRomBanks;
	uint8_t flags1;
	uint8_t flags2;
	uint8_t numRamBanks;
	uint8_t palSwitch;

	if (input.bad()) {
		fprintf(stderr, "Can't open file %s!\n", filepath);
	}

	fprintf(stdout, "Rom file %s found.\n", filepath);

	input.read((char *)&ident, 4);

	// check for NES^Z header
	if (ident != 0x1A53454E) {
		fprintf(stderr, "Invalid rom header.\n");	
	}

	fprintf(stdout, "File identifier: %X\n", ident);

	input.read((char *)&numRomBanks, 1);
	input.read((char *)&numVRomBanks, 1);

	rom.numRomBanks = (int32_t) numRomBanks;
	rom.numVRomBanks= (int32_t) numVRomBanks;

	fprintf(stdout, "Rom banks:\t%d\nVRom banks:\t%d\n", 
		rom.numRomBanks, rom.numVRomBanks);

	input.read((char *) &flags1, 1);
	input.read((char *) &flags2, 1);

	readFlags(&rom, flags1, flags2);

	fprintf(stdout, "Mapper: %d\n", rom.mapper);
	fprintf(stdout, "Flags: 0x%04X %04X\n", flags1, flags2);

	if (rom.mapper != 0x01 && rom.mapper != 0x00) {
		fprintf(stderr, "Invalid mapper 0x%X! (Only MMC3 & zero mapper are supported)\n", rom.mapper);
	}

	input.read((char *) &numRamBanks, 1);

	// for compatibility
	if (numRamBanks == 0) {
		numRamBanks = 1;
	}

	rom.numRamBanks = numRamBanks;

	fprintf(stdout, "8kB RAM banks:\t%d\n", rom.numRamBanks);

	input.read((char *) &palSwitch, 1);
	if ((palSwitch & 1) == 1) {
		rom.NTSC = false;
	} else {
		rom.NTSC = true;
	}

	fprintf(stdout, "Video mode: %s\n", rom.NTSC ? "NTSC" : "PAL");

	input.seekg(6, std::ios::cur); // skip zeros

	int32_t romSize = ROM_BANKSIZE * rom.numRomBanks;
	int32_t vromSize = VROM_BANKSIZE * rom.numVRomBanks;
	rom.romData = new char[romSize];
	rom.vromData = new char[vromSize];

	fprintf(stdout, "ROM size: %d B\t VROM size: %d B\n", 
		romSize, vromSize);

	input.read((char *)rom.romData, romSize);
	input.read((char *)rom.vromData, vromSize);

	input.close();

	return rom;
}
