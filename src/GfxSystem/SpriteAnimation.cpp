/*
 * SpriteAnimation.cpp
 *
 *  Created on: 5.4.2011
 *      Author: macjariel
 */

#include "SpriteAnimation.h"
#include "Utils/MemoryReader.h"

#include <SDL/SDL.h>

SpriteAnimation::SpriteAnimation() :
	mAnimCount(0), mCurrIdx(0), mData(0)
{
}

SpriteAnimation::~SpriteAnimation()
{
	clear();
}

void SpriteAnimation::init(const IArchive& archive, const string& filename)
{
	printf("Loading sprite animation %s.\n", filename.c_str());

	uint32_t size = archive.loadFile(filename, mData);
	Utils::MemoryReader reader(mData, size);

	mAnimCount = reader.readUint8();

	uint32_t framePositions[mAnimCount];
	for (int i = 0; i < mAnimCount; ++i)
		framePositions[i] = reader.readUint32();

	mSprites.resize(mAnimCount);
	for (int i = 0; i < mAnimCount; ++i)
	{
		mSprites[i] = new Sprite();
		mSprites[i]->init(mData + framePositions[i] + 1);
	}
}

void SpriteAnimation::clear()
{
	for (int i = 0; i < mAnimCount; ++i)
	{
		delete mSprites[i];
	}

	delete[] mData;
	mData = 0;
}
