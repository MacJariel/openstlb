/*
 * BattleMap.h
 *
 *  Created on: 1.4.2011
 *      Author: macjariel
 */

#ifndef BATTLEMAP_H_
#define BATTLEMAP_H_

#include "Setup/common.h"
#include "BattleMapStructures.h"
#include <vector>

namespace BattleMapSystem
{

struct AnimItem
{
	uint8_t unk1;
	uint16_t unk2;
	uint16_t x;
	uint16_t y;
	uint16_t idx;
};

class BattleMap
{
public:
	BattleMap();

	virtual ~BattleMap();

	void initFromFile(string fileName);

	string archiveName() const
	{
		return string(mArchiveName);
	}

	uint16_t width() const
	{
		return mWidth;
	}

	uint16_t heigth() const
	{
		return mHeight;
	}

	inline uint32_t layer1SpriteIdx(uint32_t idx) const
	{
		return mLayer1Data[idx] & 0x0FFF;
		//				mMapData[idx * 2] + 256 * (mMapData[idx * 2 + 1] & 15);
	}

	inline uint8_t level(uint32_t idx) const
	{
		return mLayer1Data[idx] >> 12;
	}

	const FilenameList& layer1SpriteNames() const
	{
		return mSpriteNames[0];
	}
	const FilenameList& layer2SpriteNames() const
	{
		return mSpriteNames[1];
	}
	const FilenameList& animNames() const
	{
		return mSpriteNames[2];
	}

	inline uint16_t layer2SpriteIdx(uint32_t idx) const
	{
		return mLayer2Data[idx];
	}

	uint32_t animCnt() const;

	void animItem(int idx, AnimItem& animItem) const;

private:
	char mArchiveName[13];

	uint16_t mWidth;
	uint16_t mHeight;

	FilenameList mSpriteNames[3];

	// Layer 1
	uint16_t* mLayer1Data;

	// Layer 2
	uint16_t* mLayer2Data;

	// Animations
	uint32_t mAnimCnt;
	uint8_t* mAnimData;

public:
	// Unknowns
	uint16_t* mUnk1Data;
	uint32_t mUnk1Cnt;
	uint16_t* mUnk2Data;
	uint32_t mUnk2Cnt;

};
}
#endif /* BATTLEMAP_H_ */
