/*
 * BattleMapGameplay.h
 *
 *  Created on: 27.4.2011
 *      Author: macjariel
 */

#ifndef __BATTLEMAPGAMEPLAY_H__
#define __BATTLEMAPGAMEPLAY_H__

class SDL_Surface;

#include "Setup/common.h"

#include "Units.h"

namespace BattleMapSystem
{

	class BattleMapGameplay
	{
	public:
		BattleMapGameplay(const string& mapName);
		virtual ~BattleMapGameplay();

		void init();

		void loop();

	private:

		void initMap();
		void initPalette();


		void drawMap();

	private:
		string mMapName;

		bool mIsRunning;
		int16_t mScrollX;
		int16_t mScrollY;

		BattleMap* mBattleMap;
		Units mUnits;

		// Variables related to drawing
		SDL_Surface* mScreen;
		uint8_t mTilesCountWidth;
		uint8_t mTilesCountHeight;


		enum TileDebugText {
			TDT_NONE = 0,
			TDT_INDEX,
			TDT_POS,

			TDT_LAST
		};
		TileDebugText mTileDebugText;
	};

}
#endif /* __BATTLEMAPGAMEPLAY_H__ */
