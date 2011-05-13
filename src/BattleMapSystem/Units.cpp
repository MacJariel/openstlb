/*
 * Units.cpp
 *
 *  Created on: 28.4.2011
 *      Author: macjariel
 */

#include "Units.h"
#include "UnitType.h"

#include "GfxSystem/Sprite.h"
#include "Utils/StringConversions.h"
#include "ResourceSystem/Archives/ArchiveMgr.h"
#include "Utils/LzwCodec.h" // temporary
namespace BattleMapSystem
{

	void Unit::updateUnitSprite(uint8_t tileType)
	{

		UnitType unitType = UnitType(type);
		//cout << unitType.gfxNormal() << endl;
		string fileName = string(unitType.gfxNormal());
		fileName += Utils::StringConversions::intToHexChar(tileType - 1);
		fileName += '0';
		fileName += Utils::StringConversions::intToHexChar(mRotation);
		//cout << fileName << ", " << (int)tileType << ", " << (int)mRotation << endl;

		uint8_t* buffer;
		int32_t size = ArchiveMgr::instance().getArchive(ArchiveMgr::T_UNITS)->loadFile(fileName.c_str(), buffer);
		if (size == -1)
		{
			//cout << "not found: " << fileName << endl;
			return;
		}
		vector<unsigned char> input(buffer, buffer + size);
		delete buffer;

		Utils::LzwCodec codec;
		vector<unsigned char> decoded;
		codec.decode(input, decoded);

		uint8_t* outBuffer = new uint8_t[decoded.size()];
		std::copy(decoded.begin(), decoded.end(), outBuffer);

		Utils::DataContainer dc(outBuffer, decoded.size());
		GfxSystem::Sprite* sprite = new GfxSystem::Sprite();

		sprite->daFlag = 1;
		sprite->init(dc);

		if (mSprite != nullptr)
			delete mSprite;

		mSprite = sprite;
	}

	void Unit::draw(SDL_Surface* screen, int posX, int posY, int level, GfxSystem::PaletteFilter* paletteFilter) const
	{
		UnitType unitType(type);
		if (unitType.isInfantry())
		{

		}

	}

}
