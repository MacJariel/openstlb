/*
 * SpritePanimation.cpp
 *
 *  Created on: 27.4.2011
 *      Author: macjariel
 */

#include "SpritePanimation.h"
#include "Utils/MemoryReader.h"
#include "ResourceSystem/Archives/IArchive.h"

#include <SDL/SDL.h>

namespace GfxSystem
{

	SpritePanimation::SpritePanimation(): mReader(nullptr)
	{
	}

	SpritePanimation::~SpritePanimation()
	{
		clear();
	}

	void SpritePanimation::init(const IArchive& archive, const string& filename)
	{
		printf("Loading sprite panimation %s.\n", filename.c_str());
		mReader = new Utils::MemoryReader(archive.loadFile(filename), true);
	}

	void SpritePanimation::clear()
	{
		delete mReader;
	}

	void SpritePanimation::setAnimData(const BattleMapSystem::BattleMap::panim_t* animData)
	{
		mAnimData = animData;
	}

	void SpritePanimation::draw(SDL_Surface* screen, int posX, int posY, int level, int offsetX, int offsetY, PaletteFilter* paletteFilter) const
	{
		mReader->setPos(0);
		uint8_t phaseCount = mReader->readUint8();
		if (phaseCount <= mAnimData->currentPhase)
		{
			cerr << "Something is wrong with panimation" << endl;
			return;
		}

		mReader->skip(mAnimData->currentPhase * 4);
		uint32_t startOffset = mReader->readUint32() + 1;
		mReader->setPos(startOffset);

		offsetY += mReader->readInt16();
		int16_t neco1 = mReader->readInt16();
		int16_t neco2 = mReader->readInt16();
		uint8_t height = mReader->readUint8();

		int baseX = -40 + posX * 80 + ((posY + 1) % 2) * 40 + mAnimData->offsetX;
		int baseY = -24 + posY * 24 - level * 18  + offsetY + mAnimData->offsetY;

		for (int screenY = baseY; screenY < baseY + height; ++screenY)
		{
			uint8_t* screenLine = (uint8_t*) screen->pixels + (screenY * screen->w);
			int screenX = baseX;
			while (true)
			{
				int lineOffsetX = mReader->readUint8() + offsetX;
				if (offsetX == -1) break;
				screenX += lineOffsetX;

				int8_t pixelCount = mReader->readUint8();
				while (pixelCount > 0)
				{
					if (screenY >= 0 && screenY < screen->h && screenX >= 0 && screenX < screen->w)
						screenLine[screenX] = mReader->readUint8();
					else
						mReader->skip(1);
					screenX++;
					pixelCount--;
				}
			}

		}
	}
}
