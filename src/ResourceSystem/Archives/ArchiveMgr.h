/*
 * ArchiveMgr.h
 *
 *  Created on: 16.4.2011
 *      Author: macjariel
 */

#ifndef __ARCHIVEMGR_H__
#define __ARCHIVEMGR_H__

#include "Setup/common.h"
#include "IArchive.h"

class ArchiveMgr
{

public:

	enum eType
	{
		T_INVALID = 0,
		T_COMMON,
		T_DEVAST,
		T_MUSIC,
		T_PUST,
		T_RESEARCH,
		T_SAMPLES,
		T_T11,
		T_TEXTS,
		T_UNITS,
		TYPES_CNT
	};

	virtual ~ArchiveMgr();

	IArchive* getArchive(eType);
	IArchive* getArchive(const string& archiveName);

	static ArchiveMgr& instance()
	{
		static ArchiveMgr singleton;
		return singleton;
	}

private:
	ArchiveMgr();
	ArchiveMgr(const ArchiveMgr&);
	const ArchiveMgr& operator=(const ArchiveMgr&);

	void loadArchives();

	eType archiveNameToType(const string& archiveName);


	IArchive* mArchives[TYPES_CNT];
};

#endif /* __ARCHIVEMGR_H__ */
