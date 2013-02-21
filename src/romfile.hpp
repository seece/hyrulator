
// A RomFile is a rom dump loaded from the disk.
#pragma once

#include <stdint.h>

struct RomFile {
		int32_t numRomBanks;	// PRG-ROM pages
		int32_t numVRomBanks;	// CHR-ROM pages
		int32_t numRamBanks;	// 8KB RAM pages 
		int32_t mapper;
		bool mirrorDirection;	// 0 horiz, 1 vertical
		bool SRAMEnabled;
		bool trainerPresent;
		bool fourScreenLayout;
		bool vsSystem;			// ???
		bool NTSC;				// false = PAL

		char * romData;
		char * vromData;
};

RomFile loadRomFile(char * filepath);
void freeRomFile(RomFile* romp);
