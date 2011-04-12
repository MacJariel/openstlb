/*
 * Palette.h
 *
 *  Created on: 10.4.2011
 *      Author: macjariel
 */

#ifndef __PALETTE_H__
#define __PALETTE_H__

#include "Setup/common.h"

class IArchive;

typedef uint8_t PaletteFilter[256];

void LoadPaletteFilter(IArchive& archive, const string& filename, PaletteFilter& filter);


enum ePaletteFilter {
	pfDarker = 0,
	NUM_PALETTE_FILTERS
};

extern PaletteFilter palettes[NUM_PALETTE_FILTERS];

#endif /* __PALETTE_H__ */
