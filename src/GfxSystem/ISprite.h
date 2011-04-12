/*
 * ISprite.h
 *
 *  Created on: 5.4.2011
 *      Author: macjariel
 */

#ifndef __ISPRITE_H__
#define __ISPRITE_H__

#include "Setup/common.h"
#include "Palette.h"

class SDL_Surface;

class ISprite
{
public:
	ISprite()	{}

	virtual ~ISprite() 	{}

	virtual void draw(SDL_Surface* screen, int posX, int posY, int level, PaletteFilter* paletteFilter = 0) const = 0;
};

#endif /* __ISPRITE_H__ */
