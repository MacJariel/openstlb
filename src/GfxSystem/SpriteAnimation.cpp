/*
 * SpriteAnimation.cpp
 *
 *  Created on: 5.4.2011
 *      Author: macjariel
 */

#include "SpriteAnimation.h"
#include "Utils/MemoryReader.h"

#include <SDL/SDL.h>

using namespace GfxSystem;

SpriteAnimation::SpriteAnimation() :
	mAnimCount(0)
{
}

SpriteAnimation::~SpriteAnimation()
{
	clear();
}

void SpriteAnimation::init(const IArchive& archive, const string& filename)
{
	printf("Loading sprite animation %s.\n", filename.c_str());

	Utils::MemoryReader reader(archive.loadFile(filename), true);

	mAnimCount = reader.readUint8();

	uint32_t framePositions[mAnimCount + 1];
	for (int i = 0; i < mAnimCount; ++i)
		framePositions[i] = reader.readUint32();
	framePositions[mAnimCount] = reader.size();

	mSprites.resize(mAnimCount);
	for (int i = 0; i < mAnimCount; ++i)
	{
		reader.setPos(framePositions[i] + 1);
		Utils::DataContainer data = reader.read(framePositions[i+1] - framePositions[i]);

		mSprites[i] = new Sprite();
		mSprites[i]->init(data);
	}
}

void SpriteAnimation::clear()
{
	for (int i = 0; i < mAnimCount; ++i)
	{
		delete mSprites[i];
	}
}

void SpriteAnimation::draw(SDL_Surface* screen, int posX, int posY, int level, int offsetX, int offsetY, PaletteFilter* paletteFilter) const
{
	mSprites[mAnimData->currentPhase]->draw(screen, posX, posY, level, offsetX, offsetY, paletteFilter);
}
