/*
 * Palette.cpp
 *
 *  Created on: 10.4.2011
 *      Author: macjariel
 */

#include "Palette.h"
#include "ResourceSystem/Archives/IArchive.h"
#include "Utils/MemoryReader.h"

#include <SDL/SDL.h>

namespace GfxSystem
{

	PaletteFilter palettes[NUM_PALETTE_FILTERS];

	void LoadPaletteFilter(IArchive& archive, const string& filename, PaletteFilter& filter)
	{
		uint8_t* data;
		uint32_t size;
		size = archive.loadFile(filename, data);
		for (int i = 0; i < 256; +i)
			filter[i] = data[i];
		delete[] data;
	}

	void LoadPalette(IArchive& archive, const string& filename, uint8_t from)
	{
		Utils::MemoryReader reader(archive.loadFile(filename), true);
		SDL_Color colors[255];
		int i = 0;
		while (reader.pos() < reader.size())
		{
			colors[i].r = reader.readUint8();
			colors[i].g = reader.readUint8();
			colors[i].b = reader.readUint8();
			++i;
		}
		SDL_SetPalette(SDL_GetVideoSurface(), SDL_PHYSPAL, colors, from, i);
	}

	SDL_Color paletteCycleBuffer1[12];
	SDL_Color paletteCycleBuffer2[12];
	uint8_t paletteCycleIndex = 0;

	void InitPaletteCycling(IArchive& archive, const string& filename)
	{
		Utils::DataContainer data(archive.loadFile(filename));
		for (int i = 0; i < 4; ++i)
		{
			paletteCycleBuffer1[i].r = data[3 * i];
			paletteCycleBuffer1[i].g = data[3 * i + 1];
			paletteCycleBuffer1[i].b = data[3 * i + 2];
		}

		for (int i = 0; i < 4; ++i)
		{
			paletteCycleBuffer1[i + 4].r = data[3 * (4 - i)];
			paletteCycleBuffer1[i + 4].g = data[3 * (4 - i) + 1];
			paletteCycleBuffer1[i + 4].b = data[3 * (4 - i) + 2];
		}

		for (int i = 0; i < 4; ++i)
		{
			paletteCycleBuffer1[i + 8].r = data[3 * i];
			paletteCycleBuffer1[i + 8].g = data[3 * i + 1];
			paletteCycleBuffer1[i + 8].b = data[3 * i + 2];
		}

		for (int i = 0; i < 5; ++i)
		{
			paletteCycleBuffer2[i].r = data[15 + 3 * i];
			paletteCycleBuffer2[i].g = data[15 + 3 * i + 1];
			paletteCycleBuffer2[i].b = data[15 + 3 * i + 2];
		}

		for (int i = 0; i < 4; ++i)
		{
			paletteCycleBuffer2[i + 5].r = data[15 + 3 * (3 - i)];
			paletteCycleBuffer2[i + 5].g = data[15 + 3 * (3 - i) + 1];
			paletteCycleBuffer2[i + 5].b = data[15 + 3 * (3 - i) + 2];
		}
	}

	void CyclePalette()
	{
		SDL_Surface* video = SDL_GetVideoSurface();
		SDL_SetPalette(video, SDL_PHYSPAL, paletteCycleBuffer1 + paletteCycleIndex, 240, 5);
		SDL_SetPalette(video, SDL_PHYSPAL, paletteCycleBuffer2 + paletteCycleIndex, 245, 5);
		paletteCycleIndex += 1;
		if (paletteCycleIndex == 8)
			paletteCycleIndex = 0;
	}
}
