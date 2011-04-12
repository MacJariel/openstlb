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

extern SDL_Surface* theScreen; // to be removed

extern TTF_Font *DEBUG_font;

Sprite::Sprite() :
	mData(0), mReader(0)
{

}

Sprite::~Sprite()
{
	clear();
}

void Sprite::init(const IArchive& archive, const string& filename)
{
	FILE* src;
	mName = filename;
	archive.loadFile(filename, mData);
	mFreeOnClear = true;
	mReader = new Utils::MemoryReader(mData, 0);
}

void Sprite::init(uint8_t* data, bool freeOnClear)
{
	mData = data;
	mFreeOnClear = freeOnClear;
	mReader = new Utils::MemoryReader(mData, 0);
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

void Sprite::draw(SDL_Surface* screen, int posX, int posY, int level, PaletteFilter* paletteFilter) const
{
	mReader->setPos(0);
	uint16_t height = mReader->readUint16();
	mReader->skip(2); // type of field (1 - 13), not necessary now
	int16_t offsetY = mReader->readInt16();
	mReader->skip(2); // unknown value, maybe two values

	int baseX = -40 + posX * 80 + ((posY + 1) % 2) * 40;
	int baseY = -24 + posY * 24 + offsetY - level * 18;

	for (int y = 0; y < height; ++y)
	{
		uint16_t offsetX = mReader->readUint16();

		uint8_t fullChunkCount = mReader->readUint8();
		uint8_t partialChunkCount = mReader->readUint8();

		int screenX = baseX + offsetX;
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
				screenLine[screenX + i] = paletteFilter ? (*paletteFilter)[mReader->readUint8()] : mReader->readUint8();
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

	int baseX = -40 + posX * 80 + ((posY + 1) % 2) * 40 + 38;
	int baseY = -24 + posY * 24 - level * 18 + 16;
	if (baseX < 0 || baseY < 0)
		return;

	int textWidth, textHeight;
	TTF_SizeText(DEBUG_font, text.c_str(), &textWidth, &textHeight);
	SDL_Surface *sText = TTF_RenderText_Solid(DEBUG_font, text.c_str(), color);

	SDL_Rect dest_rect =
	{ baseX - textWidth / 2, baseY - textHeight / 2, sText->w, sText->h };
	SDL_BlitSurface(sText, NULL, screen, &dest_rect);
	SDL_FreeSurface(sText);
}
