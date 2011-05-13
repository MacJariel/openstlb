/*
 * Units.h
 *
 *  Created on: 28.4.2011
 *      Author: macjariel
 */

#ifndef __UNITS_H__
#define __UNITS_H__

#include "Setup/common.h"

#include <SDL/SDL.h>

namespace BattleMapSystem
{

	class Unit
	{
	public:


//	private:
		string name;
		uint16_t type;
		uint16_t posX;
		uint16_t posY;

		uint8_t mRotation;
		uint8_t mTurretRotation;

		uint8_t mMaxStrength;
		uint8_t mCurStrength;

		void updateUnitSprite(uint8_t tileType);

		void draw(SDL_Surface* screen, int posX, int posY, int level, GfxSystem::PaletteFilter* paletteFilter = 0) const;


		GfxSystem::ISprite* mSprite;
	};

	typedef vector<Unit*> Units;
}

#endif /* __UNITS_H__ */
