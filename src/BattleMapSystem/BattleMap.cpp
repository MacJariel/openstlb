/*
 * BattleMap.cpp
 *
 *  Created on: 1.4.2011
 *      Author: macjariel
 */

#include "BattleMap.h"
#include "ResourceSystem/Archives/ArchiveMgr.h"
#include "Utils/MemoryReader.h"
#include "Utils/StringConversions.h"

// Graphics
#include "GfxSystem/Sprite.h"
#include "GfxSystem/SpriteAnimation.h"
#include "GfxSystem/SpritePanimation.h"

#include <cstring>

using namespace BattleMapSystem;

uint8_t BattleMap::smMapFileVersion = 18;

BattleMap::BattleMap()
{
}

BattleMap::~BattleMap()
{
}

void BattleMap::init(const string& filename)
{
	Utils::DataContainer mapFile = ArchiveMgr::instance().getArchive(ArchiveMgr::T_COMMON)->loadFile(filename);

	if (mapFile.size() == 0)
	{
		cerr << "Cannot load map file " << filename << ". File does not exist in the archive." << endl;
		return;
	}

	Utils::MemoryReader reader(mapFile);

	// ---- Read file header -----
	reader.skip(4); // unknown field
	uint32_t tileGraphicsCount = reader.readInt32();
	uint8_t mapFileVersion = reader.readUint8();

	if (mapFileVersion != smMapFileVersion)
	{
		cerr << "Cannot load map file " << filename << ". Invalid version of map file (expected v" << (int)smMapFileVersion <<
				", but encountered v" << (int) mapFileVersion << ")." << endl;
		return;
	}

	mWidth = reader.readUint16();
	mHeight = reader.readUint16();
	mArchiveName = reader.readString(13);

	// ---- Read tile graphics names -----
	for (int i = 0; i < tileGraphicsCount; ++i)
	{
		mTileGraphicsNames.push_back(reader.readString(8));
	}
	/*
	 char* layer1SpriteNames = new char[8 * layer1SpriteCnt];
	 fread(layer1SpriteNames, 1, 8 * layer1SpriteCnt, f);
	 mSpriteNames[0].init(layer1SpriteNames, layer1SpriteCnt);
	 mSpriteNames[0].setExtension(".DTA");
	 */


	// ---- Read the graphics indices of the tiles ----
	mTiles.resize(mWidth * mHeight);
	uint32_t tileCount = mWidth * mHeight;
	for (int i = 0; i < tileCount; ++i)
	{
		uint16_t tileData = reader.readUint16();
		mTiles[i].graphicsType = tileData & 0x0FFF;
		mTiles[i].level = (uint8_t) (tileData >> 12);
		const string& graphicsName = mTileGraphicsNames[mTiles[i].graphicsType];
		mTiles[i].type = graphicsName[2] - 'A' + 1;
	}

	// ---- Read the object graphics names ----
	uint32_t objectTypeCount = reader.readInt32();
	mObjectGraphicsNames.reserve(objectTypeCount);
	for (int i = 0; i < objectTypeCount; ++i)
	{
		string name = reader.readString(8);
		mObjectGraphicsNames.push_back(name);

		string prefix = name.substr(0, 2);
		if (prefix == "MR") // wall
		{
			// TODO
		}
		else if (prefix == "SO") // special object
		{
			special_object_t specialObject;
			specialObject.name = name;
			specialObject.objectIndex = i;
			specialObject.type = Utils::StringConversions::fromStdString<uint8_t>(name.substr(5, 3));
		}
	}

	// ---- Read the objects on the tiles ----
	for (int i = 0; i < tileCount; ++i)
	{
		uint16_t tileData = reader.readUint16();
		mTiles[i].objectIndex = tileData & 0x0FF;
		uint8_t objectType = (uint8_t) (tileData >> 11);
		if (objectType == 0xA) // 01010b - Special object
		{
			for(int j = 0; j < mSpecialObjects.size(); ++j)
			{
				if (mSpecialObjects[j].objectIndex == i)
				{
					/// @TODO Set object_energy
					// mTiles[i].objectEnergy = SpecialObjectParameters[ mSpecialObjects[j].type ].max_energy;
					// SpecialObjectParameters corresponds to SPECOBJ.DEF in COMMON.FS
					// @DNOTE mapa[i].policko_spec_priznaky |= 4;
					break;
				}
			}
		}
		else if (objectType == 0x1C) // 11100b - Bridge
		{
			/// @TODO
		}
	}

	// ---- Read animation type names ----
	uint32_t animTypeCount = reader.readUint32();
	if (animTypeCount > 0)
	{
		mAnimNames.reserve(animTypeCount);
		for (int i = 0; i < animTypeCount; ++i)
		{
			mAnimNames.push_back(reader.readString(8));
		}
		uint32_t animCount = reader.readUint32();
		for (int i = 0; i < animCount; ++i)
		{
			mAnimations.push_back(anim_t());
			anim_t& anim = mAnimations.back();
			anim.currentPhase = reader.readUint8();
			anim.phaseCount = reader.readUint8();
			anim.currentDelay = 0;
			anim.phaseDelay = reader.readUint8();
			anim.posX = reader.readUint16();
			anim.posY = reader.readUint16();
			anim.graphicsIndex = reader.readUint16();
		}
	}

	uint32_t panimTypeCount = reader.readUint32();
	if (panimTypeCount > 0)
	{
		mPanimNames.reserve(panimTypeCount);
		for (int i = 0; i < panimTypeCount; ++i)
		{
			mPanimNames.push_back(reader.readString(8));
		}

		uint32_t panimCount = reader.readUint32();
		for (int i = 0; i < panimCount; ++i)
		{
			mPanimations.push_back(panim_t());
			panim_t& panim = mPanimations.back();
			panim.currentPhase = reader.readUint8();
			panim.phaseCount = reader.readUint8();
			panim.phaseDelay = reader.readUint8();
			panim.currentDelay = 0;
			panim.posX = reader.readUint16();
			panim.posY = reader.readUint16();
			panim.graphicsIndex = reader.readUint16();
			panim.offsetY = reader.readInt32();
			panim.offsetX = reader.readInt32();
		}
	}

	uint32_t strategyPointCount = reader.readUint32();
	if (strategyPointCount > 0)
	{
		mStrategyPoints.reserve(strategyPointCount);
		for (int i = 0; i < strategyPointCount; ++i)
		{
			mStrategyPoints.push_back(reader.readUint16());
		}
	}

	uint32_t playerStartLocationCount = reader.readInt32();
	if (playerStartLocationCount)
	{
		for (int i = 0; i < playerStartLocationCount; ++i)
		{
			uint16_t startLocation = reader.readUint16();
			mTiles[startLocation].isPlayerStartLocation = true;
		}
	}

}

void BattleMap::loadGraphics()
{
	mTileGraphics.reserve(mTileGraphicsNames.size());
	for (vector<string>::iterator it = mTileGraphicsNames.begin(); it != mTileGraphicsNames.end(); ++it)
	{
		GfxSystem::Sprite* sprite = new GfxSystem::Sprite();
		sprite->init(*ArchiveMgr::instance().getArchive(mArchiveName), *it + ".DTA");
		mTileGraphics.push_back(sprite);
	}

	mObjectGraphics.reserve(mObjectGraphicsNames.size());
	for (vector<string>::iterator it = mObjectGraphicsNames.begin(); it != mObjectGraphicsNames.end(); ++it)
	{
		GfxSystem::Sprite* sprite = new GfxSystem::Sprite();
		sprite->init(*ArchiveMgr::instance().getArchive(mArchiveName), *it + ".DTA");
		mObjectGraphics.push_back(sprite);
	}

	mAnimGraphics.reserve(mAnimNames.size());
	for (vector<string>::iterator it = mAnimNames.begin(); it != mAnimNames.end(); ++it)
	{
		GfxSystem::SpriteAnimation* sprite = new GfxSystem::SpriteAnimation();
		sprite->init(*ArchiveMgr::instance().getArchive(mArchiveName), *it + ".ANM");
		mAnimGraphics.push_back(sprite);
	}


	mPanimGraphics.reserve(mPanimNames.size() + 1);
	/*
	{
		GfxSystem::SpritePanimation* sprite = new GfxSystem::SpritePanimation();
		sprite->init(*ArchiveMgr::instance().getArchive(ArchiveMgr::T_COMMON), "SIPKA.PNM");
		mPanimGraphics.push_back(sprite);
	}
	*/


	for (vector<string>::iterator it = mPanimNames.begin(); it != mPanimNames.end(); ++it)
	{
		GfxSystem::SpritePanimation* sprite = new GfxSystem::SpritePanimation();
		sprite->init(*ArchiveMgr::instance().getArchive(mArchiveName), *it + ".PNM");
		mPanimGraphics.push_back(sprite);
	}

}

void BattleMap::unloadGraphics()
{
	for (SpriteList::iterator it = mTileGraphics.begin(); it != mTileGraphics.end(); ++it)
	{
		delete *it;
	}
	mTileGraphics.clear();

	for (SpriteList::iterator it = mObjectGraphics.begin(); it != mObjectGraphics.end(); ++it)
	{
		delete *it;
	}
	mObjectGraphics.clear();

	for (SpriteList::iterator it = mAnimGraphics.begin(); it != mAnimGraphics.end(); ++it)
	{
		delete *it;
	}
	mAnimGraphics.clear();

	for (SpriteList::iterator it = mPanimGraphics.begin(); it != mPanimGraphics.end(); ++it)
	{
		delete *it;
	}
	mPanimGraphics.clear();
}

GfxSystem::ISprite* BattleMap::tileGraphics(TileIndex index) const
{
	return mTileGraphics[tileGraphicsType(index)];
}

int32_t BattleMapSystem::BattleMap::tileObjectType(TileIndex index) const
{
	return mTiles[index].objectIndex - 1;
}

int8_t BattleMapSystem::BattleMap::tileMovementHardness(TileIndex index) const
{
	return mTiles[index].movementHardness;
}

int8_t BattleMapSystem::BattleMap::tileLevel(TileIndex index) const
{
	return mTiles[index].level;
}

int8_t BattleMapSystem::BattleMap::tileType(TileIndex index) const
{
	return mTiles[index].type;
}

int32_t BattleMapSystem::BattleMap::tileGraphicsType(TileIndex index) const
{
	return mTiles[index].graphicsType;
}

int8_t BattleMapSystem::BattleMap::tileObjectEnergy(TileIndex index) const
{
	return mTiles[index].objectEnergy;
}

const BattleMap::PanimList& BattleMapSystem::BattleMap::panimations() const
{
	return mPanimations;
}

const BattleMap::AnimList& BattleMapSystem::BattleMap::animations() const
{
	return mAnimations;
}

GfxSystem::ISprite* BattleMap::objectGraphics(TileIndex index) const
{
	int objectType = tileObjectType(index);
	return (objectType != -1) ? mObjectGraphics[objectType] : nullptr;
}

GfxSystem::ISprite* BattleMap::animGraphics(uint32_t index) const
{
	return mAnimGraphics[index];
}

GfxSystem::ISprite* BattleMap::panimGraphics(uint32_t index) const
{
	return mPanimGraphics[index];
}

void BattleMap::injectAnimationTick()
{
	for (auto animIter = mAnimations.begin(); animIter != mAnimations.end(); ++animIter)
	{
		if (animIter->currentDelay > 0)
		{
			--animIter->currentDelay;
		}
		else
		{
			animIter->currentDelay = animIter->phaseDelay;
			++animIter->currentPhase;
			if (animIter->currentPhase >= animIter->phaseCount)
				animIter->currentPhase = 0;
		}
	}

	for (auto panimIter = mPanimations.begin(); panimIter != mPanimations.end(); ++panimIter)
	{
		if (panimIter->currentDelay > 0)
		{
			--panimIter->currentDelay;
		}
		else
		{
			panimIter->currentDelay = panimIter->phaseDelay;
			++panimIter->currentPhase;
			if (panimIter->currentPhase >= panimIter->phaseCount)
				panimIter->currentPhase = 0;
		}
	}
}
