/*
 * Sprite.cpp
 *
 *  Created on: 31.3.2011
 *      Author: macjariel
 */

#include "Sprite.h"
#include "SDL/SDL.h"
#include "ResourceSystem/Archives/IArchive.h"
#include "Utils/MemoryReader.h"

#include <SDL/SDL_ttf.h>

using namespace GfxSystem;

struct point_t {
	int8_t x;
	int8_t y;
};

point_t posunPostavicky[] = {
		{0, 0}, {8, -8}, {-9, 8}, {-10, -8}, {10, 8}, {0, -10}, {13, -21}, {-10, -2}, {-12, -25}, {13, 0}

};


extern SDL_Surface* theScreen; // to be removed

extern TTF_Font *DEBUG_font;

Sprite::Sprite() :
	daFlag(0), mData(0), mReader(0)
{

}

Sprite::~Sprite()
{
	clear();
}

void Sprite::init(const IArchive& archive, const string& filename)
{
	mName = filename;
	mReader = new Utils::MemoryReader(archive.loadFile(filename), true);
}

void Sprite::init(Utils::DataContainer& data)
{
	mReader = new Utils::MemoryReader(data, true);
}

void Sprite::clear()
{
	if (mFreeOnClear)
	{
		delete mData;
	}
	mData = 0;

	delete mReader;
	mReader = 0;
}

void Sprite::draw(SDL_Surface* screen, int posX, int posY, int level, int offsetX, int offsetY, PaletteFilter* paletteFilter) const
{
	mReader->setPos(0);
	uint16_t height = mReader->readUint16();

	if (daFlag == 0)
	{
		//mReader->skip(2); // type of field (1 - 13), not necessary now
		int something = mReader->readUint16();
	}

	//	int16_t offsetY = mReader->readInt16();
	offsetY += mReader->readInt16();

	if (daFlag != 0)
	{
		//cout << offsetY << endl;
		offsetY -= 127;
		cout << offsetY << endl;
	}

	if (daFlag == 0)
		mReader->skip(2); // unknown value, maybe two values

	int baseX = -40 + posX * 80 + ((posY + 1) % 2) * 40;
	int baseY = -24 + posY * 24 + offsetY - level * 18;
	if (daFlag)
	{
		uint8_t* start = (uint8_t*) screen->pixels + (baseY * screen->w) + baseX - 2;
		for (int i = 0; i < 5; ++i)
			start[i] = 250;

	}


	if (daFlag)
		baseY += 12;

	for (int y = 0; y < height; ++y)
	{
		uint16_t lineOffsetX = mReader->readUint16();

		uint8_t fullChunkCount = mReader->readUint8();
		uint8_t partialChunkCount = mReader->readUint8();

		int screenX = baseX + offsetX + lineOffsetX;
		int screenY = baseY + y;

		if (screenY < 0 || screenY >= screen->h)
		{
			mReader->skip(fullChunkCount * 4 + partialChunkCount * 8);
			continue;
		}

		uint32_t linePos = mReader->pos();
		uint8_t* screenLine = (uint8_t*) screen->pixels + (screenY * screen->w);

		for (int i = 0; i < fullChunkCount * 4; ++i)
		{

			if (screenX + i < 0)
			{
				mReader->skip(1);
			}
			else if ((screenX + i) >= screen->w)
			{
				mReader->setPos(linePos + fullChunkCount * 4);
				break;
			}
			else
			{
				uint8_t x = mReader->readUint8();
				//if (daFlag == 0) {
					screenLine[screenX + i] = paletteFilter ? (*paletteFilter)[x] : x;
				/*
				} else {
					//if (x >= 0x1a && x <= 0x26)
					//if (x == 0x8C)
					//if (x > 0x82 && x < 0x91)
					if (x >= 0x80 && x < 0xFD )
					{
						screenLine[screenX + i] = x;
					}
					else
					{
						screenLine[screenX + i] += x;
					}
				}
				*/
			}
		}
		screenX += fullChunkCount * 4;

		for (int i = 0; i < partialChunkCount; ++i)
		{
			uint8_t data[4];
			uint8_t mask[4];
			mReader->read(data, 4);
			mReader->read(mask, 4);
			for (int j = 0; j < 3; ++j)
			{
				if (!mask[j] && screenX + i >= 0 && screenX + i < screen->w)
				{
					if (daFlag == 0)
						screenLine[screenX + j] = paletteFilter ? (*paletteFilter)[data[j]] : data[j];
				}
			}
			screenX += 4;
		}

		/*
		 int skipPixels = screenX < 0 ? -screenX : 0;
		 if (skipPixels < fullChunkCount * 4)
		 {
		 mReader->skip(skipPixels);
		 int usePixels = fullChunkCount * 4 - skipPixels;
		 if (screenX + skipPixels + usePixels >= screen->w)
		 {
		 usePixels = screen->w - screenX - skipPixels - 1;
		 }
		 assert (screenX + skipPixels + usePixels < screen->w);
		 if (usePixels > 0)
		 mReader->read(screenLine + screenX + skipPixels, usePixels);
		 }
		 screenX += fullChunkCount * 4;
		 mReader->setPos(linePos + fullChunkCount * 4);

		 for (int i = 0; i < partialChunkCount; ++i)
		 {
		 uint8_t data[4];
		 uint8_t mask[4];
		 mReader->read(data, 4);
		 mReader->read(mask, 4);
		 if (screenX + 4 <= screen->w)
		 {
		 for (int j = 0; j < 3; ++j)
		 if (!mask[j])
		 screenLine[screenX + j] = data[j];
		 }
		 else
		 {
		 for (int j = 0; j < 3; ++j)
		 if (!mask[j] && screenX + j < screen->w)
		 screenLine[screenX + j] = data[j];
		 }
		 screenLine += 4;
		 }
		 */
	}
}

void Sprite::drawText(SDL_Surface* screen, int posX, int posY, int level, const string& text)
{
	static SDL_Color color =
	{ 255, 255, 255, 0 };

	static TTF_Font* DEBUG_font = TTF_OpenFont("arial.ttf", 12);
	if (DEBUG_font == NULL)
	{
		std::cerr << "TTF_OpenFont() Failed: " << TTF_GetError() << std::endl;
		return;
	}

	int baseX = -40 + posX * 80 + ((posY + 1) % 2) * 40 + 38;
	int baseY = -24 + posY * 24 - level * 18 + 16;
	if (baseX < 0 || baseY < 0)
		return;

	int textWidth, textHeight;
	TTF_SizeText(DEBUG_font, text.c_str(), &textWidth, &textHeight);
	SDL_Surface *sText = TTF_RenderText_Solid(DEBUG_font, text.c_str(), color);

	SDL_Rect dest_rect;
	dest_rect.x = baseX - textWidth / 2;
	dest_rect.y = baseY - textHeight / 2;
	dest_rect.w = sText->w;
	dest_rect.h = sText->h;
	SDL_BlitSurface(sText, NULL, screen, &dest_rect);
	SDL_FreeSurface(sText);
}

