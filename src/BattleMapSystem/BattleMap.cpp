/*
 * BattleMap.cpp
 *
 *  Created on: 1.4.2011
 *      Author: macjariel
 */

#include "BattleMap.h"
#include <cstring>

using namespace BattleMapSystem;

BattleMap::BattleMap():
		mAnimCnt(0)
{
	// TODO Auto-generated constructor stub

}

BattleMap::~BattleMap()
{
	delete[] mLayer1Data;
	delete[] mLayer2Data;
	delete[] mAnimData;
}

void BattleMap::initFromFile(string fileName)
{
	FILE* f = fopen(fileName.c_str(), "rb");

	// TODO: What are these unknowns
	uint32_t unk1;
	uint8_t unk2;
	uint32_t layer1SpriteCnt;

	fread(&unk1, 4, 1, f);
	fread(&layer1SpriteCnt, 4, 1, f);

	fread(&unk2, 1, 1, f);
	fread(&mWidth, 2, 1, f);
	fread(&mHeight, 2, 1, f);

	fread(mArchiveName, 1, 13, f);

	// First layer
	char* layer1SpriteNames = new char[8 * layer1SpriteCnt];
	fread(layer1SpriteNames, 1, 8 * layer1SpriteCnt, f);
	mSpriteNames[0].init(layer1SpriteNames, layer1SpriteCnt);
	mSpriteNames[0].setExtension(".DTA");

	mLayer1Data = new uint16_t[mWidth * mHeight];
	fread(mLayer1Data, 2, mWidth * mHeight, f);

	// Second layer
	{
		uint32_t spriteListSize;
		fread(&spriteListSize, 1, 4, f);
		char* spriteNames = new char[8 * spriteListSize];
		fread(spriteNames, 1, 8 * spriteListSize, f);
		mSpriteNames[1].init(spriteNames, spriteListSize);
		mSpriteNames[1].setExtension(".DTA");
		mLayer2Data = new uint16_t[mWidth * mHeight];
		fread(mLayer2Data, 2, mWidth * mHeight, f);
	}

	// Animations
	{
		uint32_t animNameCnt;
		char* animNames;
		fread(&animNameCnt, 1, 4, f);
		animNames = new char[8 * animNameCnt];
		fread(animNames, 1, 8 * animNameCnt, f);
		mSpriteNames[2].init(animNames, animNameCnt);
		mSpriteNames[2].setExtension(".ANM");

		if (animNameCnt != 0)
		{
			fread(&mAnimCnt, 1, 4, f);
			printf("mAnimCnt: %d\n", mAnimCnt);
			mAnimData = new uint8_t[9 * mAnimCnt];
			fread(mAnimData, 1, 9 * mAnimCnt, f);
		}
	}

	// 2nd animations (TODO)
	{
		uint32_t animNameCnt;
		fread(&animNameCnt, 1, 4, f);
		if (animNameCnt != 0)
		{
			fseek(f, 8 * animNameCnt, SEEK_CUR);

			uint32_t animCnt;
			fread(&animCnt, 1, 4, f);

			fseek(f, 17 * animCnt, SEEK_CUR);
		}
	}
	// 1st unknown layer
	{
		fread(&mUnk1Cnt, 1, 4, f);
		if (mUnk1Cnt != 0)
		{
			mUnk1Data = new uint16_t[mUnk1Cnt];
			fread(mUnk1Data, 2, mUnk1Cnt, f);
		}
	}

	{
		fread(&mUnk2Cnt, 1, 4, f);
		if (mUnk2Cnt != 0)
		{
			mUnk2Data = new uint16_t[mUnk2Cnt];
			fread(mUnk2Data, 2, mUnk2Cnt, f);
		}
	}
	fclose(f);
}

uint32_t BattleMap::animCnt() const
{
	return mAnimCnt;
}

void BattleMap::animItem(int idx, AnimItem& animItem) const
{
	memcpy(&animItem.unk1, mAnimData + 9 * idx + 0, 1);
	memcpy(&animItem.unk2, mAnimData + 9 * idx + 1, 2);
	memcpy(&animItem.x, mAnimData + 9 * idx + 3, 2);
	memcpy(&animItem.y, mAnimData + 9 * idx + 5, 2);
	memcpy(&animItem.idx, mAnimData + 9 * idx + 7, 2);
}

