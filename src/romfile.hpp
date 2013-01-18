
// A RomFile is a rom dump loaded from the disk.
#pragma once

#include <stdint.h>

class RomFile {
	public:
		RomFile(const char * filepath);

		uint32_t numRomBanks;	// PRG-ROM pages
		uint32_t numVRomBanks;	// CHR-ROM pages
		uint32_t numRamBanks;	// 8KB RAM pages 
		uint32_t mapper;
		bool horizontalMirror;
		bool verticalMirror;
		bool SRAMEnabled;
		bool trainerPresent;
		bool fourScreenLayout;
		bool NTSC;				// false = PAL
	private:
};
