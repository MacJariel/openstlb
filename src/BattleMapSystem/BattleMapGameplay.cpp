/*
 * BattleMapGameplay.cpp
 *
 *  Created on: 27.4.2011
 *      Author: macjariel
 */

#include "BattleMapGameplay.h"
#include "BattleMap.h"
#include "UnitType.h"

#include "ResourceSystem/Archives/ArchiveMgr.h"
#include "GfxSystem/Palette.h"
#include "GfxSystem/ISprite.h"
#include "GfxSystem/Sprite.h"	// for Sprite::drawText()
#include "GfxSystem/SpriteAnimation.h"
#include "GfxSystem/SpritePanimation.h"

#include "Utils/StringConversions.h"

#include <SDL/SDL.h>

using namespace BattleMapSystem;

BattleMapGameplay::BattleMapGameplay(const string& mapName) :
	mMapName(mapName), mIsRunning(false), mScrollX(0), mScrollY(0), mBattleMap(nullptr), mTileDebugText(TDT_NONE)
{

}

BattleMapGameplay::~BattleMapGameplay()
{
}

void BattleMapGameplay::init()
{
	// Init map
	mBattleMap = new BattleMap();
	mBattleMap->init(mMapName);
	mBattleMap->loadGraphics();

	// Init palette
	GfxSystem::LoadPalette(*ArchiveMgr::instance().getArchive(mBattleMap->archiveName()), "MAP.PAL", 0);
	GfxSystem::LoadPalette(*ArchiveMgr::instance().getArchive(ArchiveMgr::T_COMMON), "UNITS.PAL", 128);
	GfxSystem::LoadPalette(*ArchiveMgr::instance().getArchive(ArchiveMgr::T_COMMON), "SYSTEM.PAL", 224);

	GfxSystem::LoadPaletteFilter(*ArchiveMgr::instance().getArchive(mBattleMap->archiveName()), "DARKER.PAL", GfxSystem::palettes[GfxSystem::pfDarker]);

	GfxSystem::InitPaletteCycling(*ArchiveMgr::instance().getArchive(mBattleMap->archiveName()), "CYCLE.PAL");

	mScreen = SDL_GetVideoSurface();
	mTilesCountWidth = (mScreen->w / 80) + 2;
	mTilesCountHeight = (mScreen->h / 24) + 4;

	// TMP: load unit types
	UnitType::loadUnitTypes();

	// TMP: load some units
	{
		Unit* u1 = new Unit();
		u1->name = "U1";
		u1->type = 1;
		u1->posX = 7;
		u1->posY = 30;
		u1->mSprite = nullptr;
		mUnits.push_back(u1);
	}

	{
		Unit* u1 = new Unit();
		u1->name = "U2";
		u1->type = 35;
		u1->posX = 8;
		u1->posY = 30;
		u1->mSprite = nullptr;
		mUnits.push_back(u1);
	}

}

void BattleMapGameplay::loop()
{
	mIsRunning = true;

	// TODO: somewhere else
	SDL_EnableKeyRepeat(300, 10);

	SDL_Event event;
	while (mIsRunning)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					--mScrollX;
					break;
				case SDLK_RIGHT:
					++mScrollX;
					break;
				case SDLK_UP:
					mScrollY -= 2;
					break;
				case SDLK_DOWN:
					mScrollY += 2;
					break;
				case SDLK_q:
					mIsRunning = false;
					break;
				case SDLK_d:
					mTileDebugText = (TileDebugText)((mTileDebugText + 1) % TDT_LAST);
				}
				break;
			case SDL_QUIT:
				mIsRunning = false;
				break;
			}
		}

		SDL_Delay(10);
		// cout << (int) mScrollX << ", " << (int) mScrollY << endl;
		drawMap();
		mBattleMap->injectAnimationTick();
		GfxSystem::CyclePalette();

		// TMP: update units positions
		for (int i = 0; i < mUnits.size(); ++i)
		{
			mUnits[i]->posX = mScrollX + 10 + i;
			mUnits[i]->posY = mScrollY + 10;
		}
	}
}

void BattleMapGameplay::drawMap()
{
	SDL_FillRect(mScreen, NULL, 0);

	for (int tileIdx = 0; tileIdx < mBattleMap->width() * mBattleMap->heigth(); ++tileIdx)
	{
		int tileX = tileIdx % mBattleMap->width();
		int tileY = tileIdx / mBattleMap->width();

		if (tileX < mScrollX || tileX >= mScrollX + mTilesCountWidth || tileY < mScrollY || tileY >= mScrollY
				+ mTilesCountHeight)
			continue;

		// Darker filter on the edges

		GfxSystem::PaletteFilter* paletteFilter = 0;

		if (tileX < 1 || tileX >= mBattleMap->width() - 1 || tileY < 7 || tileY >= mBattleMap->heigth() - 7)
		{
			paletteFilter = &GfxSystem::palettes[GfxSystem::pfDarker];
		}

		int tileScreenX = tileX - mScrollX;
		int tileScreenY = tileY - mScrollY;

		GfxSystem::ISprite* tileSprite = mBattleMap->tileGraphics(tileIdx);
		tileSprite->draw(mScreen, tileScreenX, tileScreenY, mBattleMap->tileLevel(tileIdx), 0, 0, paletteFilter);

		GfxSystem::ISprite* objectSprite = mBattleMap->objectGraphics(tileIdx);
		if (objectSprite != nullptr)
			objectSprite->draw(mScreen, tileScreenX, tileScreenY, mBattleMap->tileLevel(tileIdx), 0, 0, paletteFilter);

	}

	const BattleMap::AnimList& animations = mBattleMap->animations();
	for (auto animIter = animations.begin(); animIter != animations.end(); ++animIter)
	{
		if (animIter->posX < mScrollX || animIter->posX >= mScrollX + mTilesCountWidth || animIter->posY < mScrollY
				|| animIter->posY >= mScrollY + mTilesCountHeight)
			continue;
		TileIndex tileIdx = mBattleMap->posToTileIndex(animIter->posX, animIter->posY);
		int tileScreenX = animIter->posX - mScrollX;
		int tileScreenY = animIter->posY - mScrollY;

		auto animSprite = static_cast<GfxSystem::SpriteAnimation*> (mBattleMap->animGraphics(animIter->graphicsIndex));
		animSprite->setAnimData(&*animIter);
		animSprite->draw(mScreen, tileScreenX, tileScreenY, mBattleMap->tileLevel(tileIdx), 0, 0); // TODO: palette filter is not applied

	}

	const BattleMap::PanimList& panimations = mBattleMap->panimations();
	for (auto panimIter = panimations.begin(); panimIter != panimations.end(); ++panimIter)
	{
		if (panimIter->posX < mScrollX || panimIter->posX >= mScrollX + mTilesCountWidth || panimIter->posY < mScrollY
				|| panimIter->posY >= mScrollY + mTilesCountHeight)
			continue;
		TileIndex tileIdx = mBattleMap->posToTileIndex(panimIter->posX, panimIter->posY);
		int tileScreenX = panimIter->posX - mScrollX;
		int tileScreenY = panimIter->posY - mScrollY;

		auto panimSprite = static_cast<GfxSystem::SpritePanimation*> (mBattleMap->panimGraphics(
				panimIter->graphicsIndex));
		panimSprite->setAnimData(&*panimIter);
		panimSprite->draw(mScreen, tileScreenX, tileScreenY, mBattleMap->tileLevel(tileIdx), 0, 0); // TODO: palette filter is not applied

	}

	for (auto unitIter = mUnits.begin(); unitIter != mUnits.end(); ++unitIter)
	{

		Unit* unit = *unitIter;
		cout << "UNIT: " << unit->posX << ", " << unit->posY << endl;
		if (unit->posX < mScrollX || unit->posX >= mScrollX + mTilesCountWidth || unit->posY < mScrollY || unit->posY
				>= mScrollY + mTilesCountHeight)
			continue;

		TileIndex tileIdx = mBattleMap->posToTileIndex(unit->posX, unit->posY);
		int tileScreenX = unit->posX - mScrollX;
		int tileScreenY = unit->posY - mScrollY;

		unit->updateUnitSprite(mBattleMap->tileType(tileIdx));
		if (unit->mSprite != nullptr)
			unit->mSprite->draw(mScreen, tileScreenX, tileScreenY, mBattleMap->tileLevel(tileIdx), 0, 0); // TODO: palette filter is not applied
		GfxSystem::Sprite::drawText(mScreen, tileScreenX, tileScreenY, mBattleMap->tileLevel(tileIdx), "*");
		unit->mRotation = (unit->mRotation + 1) % 16;
	}

	if (mTileDebugText != TDT_NONE)
	{
		for (int tileIdx = 0; tileIdx < mBattleMap->width() * mBattleMap->heigth(); ++tileIdx)
		{
			int tileX = tileIdx % mBattleMap->width();
			int tileY = tileIdx / mBattleMap->width();

			if (tileX < mScrollX || tileX >= mScrollX + mTilesCountWidth || tileY < mScrollY || tileY >= mScrollY
					+ mTilesCountHeight)
				continue;

			int tileScreenX = tileX - mScrollX;
			int tileScreenY = tileY - mScrollY;

			string text;
			switch (mTileDebugText)
			{
			case TDT_INDEX:
				text = Utils::StringConversions::toStdString(tileIdx);
				break;
			case TDT_POS:
				text = Utils::StringConversions::toStdString(tileX) + ", " + Utils::StringConversions::toStdString(
						tileY);
				break;
			}

			GfxSystem::Sprite::drawText(mScreen, tileScreenX, tileScreenY, mBattleMap->tileLevel(tileIdx), text);
		}
	}
	/*
	 for (int i = 0; i < battleMap.width() * battleMap.heigth(); ++i)
	 {
	 int x = i % battleMap.width();
	 int y = i / battleMap.width();

	 if (x < xFrom || x >= xTo || y < yFrom || y >= yTo)
	 continue;

	 uint16_t idx = battleMap.layer2SpriteIdx(i);
	 uint8_t lower = idx & 0xFF;
	 uint8_t upper = (idx >> 8);
	 if (lower != 0)
	 {
	 lower--;
	 Sprite* sprite = layer2Sprites[lower];
	 sprite->draw(screen, x - xFrom, y - yFrom, battleMap.level(i));
	 }
	 else
	 {
	 s->draw(screen, x - xFrom, y - yFrom, battleMap.level(i));

	 }
	 }

	 for (int i = 0; i < battleMap.animCnt(); ++i)
	 {
	 BattleMapSystem::AnimItem animItem;
	 battleMap.animItem(i, animItem);

	 if (animItem.x < xFrom || animItem.x >= xTo || animItem.y < yFrom || animItem.y >= yTo)
	 continue;

	 SpriteAnimation* sprite = animSprites[animItem.idx];
	 sprite->draw(screen, animItem.x - xFrom, animItem.y - yFrom, battleMap.level(i));
	 sprite->next();
	 }

	 for (int i = 0; i < battleMap.width() * battleMap.heigth(); ++i)
	 {
	 int x = i % battleMap.width();
	 int y = i / battleMap.width();

	 if (x < xFrom || x >= xTo || y < yFrom || y >= yTo)
	 continue;

	 //uint16_t idx = battleMap.layer2SpriteIdx(i) >> 12;
	 uint16_t idx = battleMap.layer2SpriteIdx(i) >> 8;
	 //uint16_t idx = battleMap.layer2SpriteIdx(i) & 0xFF;
	 //blitTileNumber(screen, posX, posY, x, y, battleMap.level(i), idx);
	 //blitTileNumber(screen, posX, posY, x, y, battleMap.level(i), i);

	 /*
	 // unk1
	 for (int j = 0; j < battleMap.mUnk1Cnt; ++j)
	 {
	 uint16_t data = *(battleMap.mUnk1Data + j);
	 if (data == i)
	 {
	 char aa[10];
	 sprintf(aa, "U1: %d, %d", i, j);
	 blitTileString(screen, posX, posY, x, y, battleMap.level(i), aa);
	 }
	 }

	 // unk2
	 for (int j = 0; j < battleMap.mUnk2Cnt; ++j)
	 {
	 uint16_t data = *(battleMap.mUnk2Data + j);
	 if (data == i)
	 {
	 char aa[10];
	 sprintf(aa, "U2: %d, %d", i, j);
	 blitTileString(screen, posX, posY, x, y, battleMap.level(i), aa);
	 }
	 }
	 */

	// }
	SDL_Flip(mScreen);
}
