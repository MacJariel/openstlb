/*
 * Sprite.h
 *
 *  Created on: 31.3.2011
 *      Author: macjariel
 */

#ifndef SPRITE_H_
#define SPRITE_H_

#include "Setup/common.h"
#include "ISprite.h"
#include "ResourceSystem/Archives/IArchive.h"

namespace Utils {
	class MemoryReader;
}


class SDL_Surface;

class Sprite: public ISprite {
public:
	Sprite();
	virtual ~Sprite();

	void init(const IArchive& archive, const string& filename);
	void init(uint8_t* data, bool freeOnClear = false);
	void clear();

	virtual void draw(SDL_Surface* screen, int posX, int posY, int level, PaletteFilter* paletteFilter = 0) const;

	static void drawText(SDL_Surface* screen, int posX, int posY, int level, const string& text);

private:

	uint8_t* mData;
	bool mFreeOnClear;

	string mName;
	Utils::MemoryReader* mReader;
};

#endif /* SPRITE_H_ */
