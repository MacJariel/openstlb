/*
 * SpriteAnimation.h
 *
 *  Created on: 5.4.2011
 *      Author: macjariel
 */

#ifndef __SPRITEANIMATION_H__
#define __SPRITEANIMATION_H__

#include "Setup/common.h"
#include "Sprite.h"

#include <vector>

class SpriteAnimation: public ISprite
{
public:
	SpriteAnimation();
	virtual ~SpriteAnimation();

	void init(const IArchive& archive, const string& filename);
	void clear();

	virtual void draw(SDL_Surface* screen, int posX, int posY, int level, PaletteFilter* paletteFilter = 0) const
	{
		mSprites[mCurrIdx]->draw(screen, posX, posY, level);
	}

	void next()
	{
		mCurrIdx = (mCurrIdx + 1) % mAnimCount;
	}

private:
	uint8_t mAnimCount;
	uint8_t mCurrIdx;
	vector<Sprite*> mSprites;

	uint8_t* mData;
};

#endif /* __SPRITEANIMATION_H__ */
