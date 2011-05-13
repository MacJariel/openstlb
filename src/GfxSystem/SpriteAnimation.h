/*
 * SpriteAnimation.h
 *
 *  Created on: 5.4.2011
 *      Author: macjariel
 */

#ifndef __SPRITEANIMATION_H__
#define __SPRITEANIMATION_H__

#include "Setup/common.h"
#include "Sprite.h"

#include "BattleMapSystem/BattleMap.h" // for anim_t struct (maybe it should be pulled to another header)

#include <vector>

namespace GfxSystem
{

	class SpriteAnimation: public ISprite
	{
	public:
		SpriteAnimation();
		virtual ~SpriteAnimation();

		void init(const IArchive& archive, const string& filename);
		void clear();

		virtual void draw(SDL_Surface* screen, int posX, int posY, int level, int offsetX, int offsetY, PaletteFilter* paletteFilter = 0) const;

/*		{
			mSprites[mCurrIdx]->draw(screen, posX, posY, level);
		}
*/
		void setAnimData(const BattleMapSystem::BattleMap::anim_t* animData)
		{
			mAnimData = animData;
		}


	private:
		uint8_t mAnimCount;
		vector<Sprite*> mSprites;
		const BattleMapSystem::BattleMap::anim_t* mAnimData;
	};
}
#endif /* __SPRITEANIMATION_H__ */
