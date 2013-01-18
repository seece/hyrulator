
#include <stdint.h>
#include <cstdio>
#include <iostream>
#include <fstream>
#include <ios>
#include "romfile.hpp"

RomFile::RomFile(const char * filepath) {
	std::ifstream input (filepath, std::ifstream::in);

	if (input.bad()) {
		fprintf(stderr, "Can't open file %s!\n", filepath);
	}

	fprintf(stdout, "Rom file %s found.\n", filepath);

	input.close();
}
