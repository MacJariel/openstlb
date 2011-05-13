/*
 * UnitType.h
 *
 *  Created on: 16.4.2011
 *      Author: macjariel
 */

#ifndef __UNITTYPE_H__
#define __UNITTYPE_H__

#include "Setup/common.h"

namespace BattleMapSystem
{
	class UnitTypeMgr;

	class UnitType
	{
	public:

		enum eUnitKind
		{
			ukNormal = 0, ukTurret = 1, ukInfantry = 2, ukAir = 4
		};

		UnitType(int id);
		virtual ~UnitType();

		const char* name() const;
		const char* gfxArtwork() const;
		const char* gfxNormal() const;
		const char* gfxTurret() const;
		const char* gfxIcon() const;

		eUnitKind kind() const;

		uint8_t unk() const;

		bool isInfantry() const;

		static void loadUnitTypes();

	private:
		friend class UnitTypeMgr;

		UnitType(int id, Utils::MemoryReader& reader);
		UnitType(const UnitType&);
		UnitType& operator=(UnitType&);

		enum eAttackType {
			atLight = 0,
			atHeavy,
			atAir,
			atObjects,
		};

		int mId;
		string mName;
		string mGfxArtworkName;
		string mGfxBaseName;
		string mGfxTurretName;
		string mGfxIconName;
		uint8_t mTypeFlags;
		uint8_t mMaxAP;
		uint8_t mPocetFazPohybu;	//TODO: rename
		uint8_t mBodyPrechodu;		//TODO: rename
		uint8_t mStrength;
		uint8_t mAttack[4];
		uint8_t mDefense;
		uint8_t mAccuracy;
		uint8_t mInitiative;		//TODO: maybe rename
		uint8_t mAttackRange;
		uint8_t mAttackFlags;
		uint8_t mAttackAP[3];
		string mAttackPanmName[3];
		string mAttackAnmName[3];
		string mAttackExplosionName[3];
		uint8_t mAttackPhaseCount[3];
		uint8_t mProjectileFlags;
		string mGfxProjectileName;
		uint8_t mSpecFlags;
		string mSpecAnimName;
		uint8_t mSpecFazy;			//TODO: rename
		uint8_t mSpecAb;			//TODO: rename
		uint8_t mSpecRange;
		uint8_t mSpecDamage;
		uint8_t mSpecTransformTarget;
		uint8_t mVisionRange;
		uint8_t mMaxPersons;
		uint8_t mEntrenchmentTime;
		/// ...
	};


	class UnitTypeMgr
	{
	public:
		void loadUnitTypes();
		void unloadUnitTypes();

		const UnitType& unitType(int unitTypeId) const;

		static UnitTypeMgr& instance()
		{
			static UnitTypeMgr singleton;
			return singleton;
		}

	private:
		UnitTypeMgr();
		~UnitTypeMgr();
		UnitTypeMgr(const UnitTypeMgr&);
		UnitTypeMgr& operator=(UnitTypeMgr&);

		vector<UnitType*> mUnitTypes;
	};

}
#endif /* __UNITTYPE_H__ */
