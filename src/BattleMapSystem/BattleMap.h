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

	typedef uint32_t TileIndex;

	class BattleMap
	{
	public:

		struct anim_t {
			uint8_t phaseCount;
			uint8_t phaseDelay;
			uint8_t currentPhase;
			uint8_t currentDelay;
			uint16_t posX;
			uint16_t posY;
			uint16_t graphicsIndex;
		};

		typedef vector<anim_t> AnimList;

		struct panim_t {
			uint8_t phaseCount;
			uint8_t phaseDelay;
			uint8_t currentPhase;
			uint8_t currentDelay;
			uint16_t posX;
			uint16_t posY;
			int32_t offsetX;
			int32_t offsetY;
			uint16_t graphicsIndex;
		};

		typedef vector<panim_t> PanimList;


		BattleMap();

		virtual ~BattleMap();

		/**
		 * Initializes the map from the specified .DTA file in the COMMON.FS archive.
		 * @param filename
		 */
		void init(const string& filename);

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

		/** @name Properties of map tiles
		 * These methods return various properties of tiles at specified index.
		 */
		//@{

		/**
		 * Returns the graphics type of the specified tile.
		 */
		int32_t tileGraphicsType(TileIndex index) const;


		/**
		 * Returns the type index of the object on the specified tile, or -1 if
		 * no object is on the tile.
		 */
		int32_t tileObjectType(TileIndex index) const;

		/**
		 * Returns the level of the specified tile.
		 */
		int8_t tileLevel(TileIndex index) const;

		/**
		 * Returns the type (scope) of the tile.
		 */
		int8_t tileType(TileIndex index) const;

		/**
		 * Returns the energy of the object at the specified tile.
		 */
		int8_t tileObjectEnergy(TileIndex index) const;

		/**
		 * Returns the hardness of the movement at the specified tile.
		 */
		int8_t tileMovementHardness(TileIndex index) const;

		/**
		 * Returns the list of animations on the map.
		 */
		const AnimList& animations() const;

		/**
		 * Returns the list of panimations on the map.
		 */
		const PanimList& panimations() const;


		//@}

		/** @name Sprite manipulation
		 * These methods are related to sprites.
		 */
		//@{

		/**
		 * Loads all the graphics that is used in this map.
		 */
		void loadGraphics();

		/**
		 * Unloads all the graphics that is used in this map.
		 */
		void unloadGraphics();

		/**
		 * Returns a tile graphics of given tile.
		 */
		GfxSystem::ISprite* tileGraphics(TileIndex tileIndex) const;

		/**
		 * Returns the graphics of the object on the specified tile, or nullptr if no object is present.
		 */
		GfxSystem::ISprite* objectGraphics(TileIndex tileIndex) const;

		/**
		 * Returns the graphics of the animation at the given index.
		 */
		GfxSystem::ISprite* animGraphics(uint32_t index) const;

		/**
		 * Returns the graphics of the panimation at the given index.
		 */
		GfxSystem::ISprite* panimGraphics(uint32_t index) const;


		//@}

		inline TileIndex posToTileIndex(uint16_t posX, uint16_t posY)
		{
			return posX + posY * mWidth;
		}

		/**
		 * Injects a tick to the animation simulation and updates animation structures.
		 */
		void injectAnimationTick();

	private:

		struct tile_t
		{
			uint16_t graphicsType;
			uint8_t level;
			uint8_t type;
			uint16_t objectIndex;
			uint8_t objectEnergy;
			uint8_t movementHardness;
			bool isPlayerStartLocation;
		};

		struct special_object_t {
			string name;
			uint16_t objectIndex;
			uint16_t collapseIndex;
			uint16_t type;
		};

		string mArchiveName;

		uint16_t mWidth;
		uint16_t mHeight;
public:
		vector<string> mTileGraphicsNames;
		vector<string> mObjectGraphicsNames;
		vector<string> mAnimNames;
		vector<string> mPanimNames;

		vector<tile_t> mTiles;
		vector<special_object_t> mSpecialObjects;
		AnimList mAnimations;
		PanimList mPanimations;
		vector<uint16_t> mStrategyPoints;

		// Graphics
		typedef vector<GfxSystem::ISprite*> SpriteList;

		SpriteList mTileGraphics;
		SpriteList mObjectGraphics;
		SpriteList mAnimGraphics;
		SpriteList mPanimGraphics;

		static uint8_t smMapFileVersion;
	};
}
#endif /* BATTLEMAP_H_ */
