/*
 * UnitType.cpp
 *
 *  Created on: 16.4.2011
 *      Author: macjariel
 */

#include "UnitType.h"
#include "ResourceSystem/Archives/ArchiveMgr.h"
#include "Utils/MemoryReader.h"

using namespace BattleMapSystem;

namespace
{
	enum
	{
		RECORD_SIZE = 206
	};

	enum eKindMasks
	{
		kmHasTurret = 1 << 0,
		kmInfantry = 1 << 1,	// can walk through woods, sometimes more persons in gfx
		kmAirUnit = 1 << 2,
		kmLevitation = 1 << 3,	// though not air unit, can walk over water/cliffs (Ghost, Predator)
		kmLightUnit = 1 << 4,
		kmHeavyUnit = 1 << 5,
		kmBioUnit = 1 << 6,		// biological unit (in sense of A_SOUNDS.DEF)
		kmUnk3 = 1 << 7			// regards to unit placement (VratCiNeprekaza) TODO: only Demon
	};

} // namespace


#if 0
UnitType::UnitType(int id) :
	mId(id)
{
}

UnitType::~UnitType()
{
}

const char* UnitType::gfxIcon() const
{
	return (const char*)&smUnitTypesData[mId * RECORD_SIZE + 0x31];
}

const char* UnitType::gfxNormal() const
{
	return (const char*)&smUnitTypesData[mId * RECORD_SIZE + 0x25];
}

const char* UnitType::gfxArtwork() const
{
	return (const char*)&smUnitTypesData[mId * RECORD_SIZE + 0x1C];
}

const char* UnitType::gfxTurret() const
{
	return (const char*)&smUnitTypesData[mId * RECORD_SIZE + 0x2B];
}

const char* UnitType::name() const
{
	return (const char*)&smUnitTypesData[mId * RECORD_SIZE];
}

UnitType::eUnitKind UnitType::kind() const
{
	return (eUnitKind)(smUnitTypesData[mId * RECORD_SIZE + 0x3A] & 7);
}
uint8_t UnitType::unk() const
{
	return smUnitTypesData[mId * RECORD_SIZE + 0x3A];
}

bool UnitType::isInfantry() const
{
	return smUnitTypesData[mId * RECORD_SIZE + 0x3A] & kmInfantry;
}

#endif


UnitType::UnitType(int id, Utils::MemoryReader& reader):
		mId(id)
{



}



void UnitTypeMgr::loadUnitTypes()
{
	Utils::MemoryReader reader(ArchiveMgr::instance().getArchive(ArchiveMgr::T_COMMON)->loadFile("JEDNOTKY.DEF"), true);
	reader.setPos(0);
	mUnitTypes.reserve(100);
	for (int id = 0; id < 100; ++id)
	{
		mUnitTypes.push_back(new UnitType(id, reader));
	}
}

void UnitTypeMgr::unloadUnitTypes()
{
	for (auto it = mUnitTypes.begin(); it != mUnitTypes.end(); ++it)
		delete *it;
	mUnitTypes.clear();
}

const UnitType& UnitTypeMgr::unitType(int unitTypeId) const
{
	return *mUnitTypes.at(unitTypeId);
}
