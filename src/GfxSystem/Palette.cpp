/*
 * Palette.cpp
 *
 *  Created on: 10.4.2011
 *      Author: macjariel
 */

#include "Palette.h"
#include "ResourceSystem/Archives/IArchive.h"

PaletteFilter palettes[NUM_PALETTE_FILTERS];

void LoadPaletteFilter(IArchive& archive, const string& filename, PaletteFilter& filter)
{
	uint8_t* data;
	uint32_t size;
	size = archive.loadFile(filename, data);
	if (size == 256)
	{
		std::copy(data, data + 256, filter);
	}
	delete [] data;
}
