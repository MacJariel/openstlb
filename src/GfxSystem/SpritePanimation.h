/*
 * SpritePanimation.h
 *
 *  Created on: 27.4.2011
 *      Author: macjariel
 */

#ifndef __SPRITEPANIMATION_H__
#define __SPRITEPANIMATION_H__

#include "Setup/common.h"
#include "ISprite.h"

#include "BattleMapSystem/BattleMap.h" // for anim_t struct (maybe it should be pulled to another header)

namespace GfxSystem
{

	class SpritePanimation: public GfxSystem::ISprite
	{
	public:
		SpritePanimation();
		virtual ~SpritePanimation();

		void init(const IArchive& archive, const string& filename);
		void clear();

		void setAnimData(const BattleMapSystem::BattleMap::panim_t* animData);

		virtual void draw(SDL_Surface* screen, int posX, int posY, int level, int offsetX, int offsetY, PaletteFilter* paletteFilter = 0) const;

	private:
		Utils::MemoryReader* mReader;

		const BattleMapSystem::BattleMap::panim_t* mAnimData;
	};

}

#endif /* __SPRITEPANIMATION_H__ */
