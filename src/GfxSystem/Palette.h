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

namespace GfxSystem
{
	class PaletteFilter: public vector<uint8_t>
	{
	public:
		PaletteFilter() :
			vector<uint8_t> (256)
		{
		}
	};

	//typedef uint8_t PaletteFilter[256];

	void InitPaletteCycling(IArchive& archive, const string& filename);

	void CyclePalette();

	void LoadPaletteFilter(IArchive& archive, const string& filename, PaletteFilter& filter);

	void LoadPalette(IArchive& archive, const string& filename, uint8_t from);

	enum ePaletteFilter
	{
		pfDarker = 0, NUM_PALETTE_FILTERS
	};

	extern PaletteFilter palettes[NUM_PALETTE_FILTERS];
}
#endif /* __PALETTE_H__ */
