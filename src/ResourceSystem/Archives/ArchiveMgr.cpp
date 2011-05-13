/*
 * ArchiveMgr.cpp
 *
 *  Created on: 16.4.2011
 *      Author: macjariel
 */

#include "ArchiveMgr.h"
#include "FsArchive.h"
#include "FsuArchive.h"

namespace
{
	string archiveDir = "DATA/";

	bool archiveTypes[] =
	{ false, false, false, false, false, false, false, false, false, true, false };

	const char* archiveFilenames[] =
	{ 0, "COMMON", "DEVAST", "MUSIC", "PUST", "RESEARCH", "SAMPLES", "T11", "TEXTS", "UNITS", 0 };

} // namespace

ArchiveMgr::ArchiveMgr()
{
	loadArchives();
}

ArchiveMgr::~ArchiveMgr()
{
	for (int i = 1; i < TYPES_CNT; ++i)
	{
		delete mArchives[i];
	}
}

IArchive* ArchiveMgr::getArchive(eType type)
{
	if (type == T_INVALID || type >= TYPES_CNT)
		return 0;
	return mArchives[type];
}

IArchive* ArchiveMgr::getArchive(const string& archiveName)
{
	eType type = archiveNameToType(archiveName);
	return getArchive(type);
}

void ArchiveMgr::loadArchives()
{
	mArchives[0] = 0;
	for (int i = 1; i < TYPES_CNT; ++i)
	{
		if (archiveTypes[i])
			mArchives[i] = new FsuArchive(archiveDir + string(archiveFilenames[i]) + ".FSU");
		else
			mArchives[i] = new FsArchive(archiveDir + string(archiveFilenames[i]) + ".FS");

		mArchives[i]->init();
	}
}

ArchiveMgr::eType ArchiveMgr::archiveNameToType(const string& archiveName)
{
	for (int i = 1; i < TYPES_CNT; ++i)
	{
		if (archiveName == archiveFilenames[i])
			return (eType)i;
	}
	return T_INVALID;
}
