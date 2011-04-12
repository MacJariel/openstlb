/*
 * FsArchive.h
 *
 *  Created on: 4.4.2011
 *      Author: macjariel
 */

#ifndef __FSARCHIVE_H__
#define __FSARCHIVE_H__

#include "Setup/common.h"
#include "IArchive.h"

class FsArchive: public IArchive {
public:

	enum eType
	{
		T_COMMON = 0,
		T_DEVAST,
		T_MUSIC,
		T_PUST,
		T_RESEARCH,
		T_SAMPLES,
		T_T11,
		T_TEXTS,

		TYPES_CNT
	};

	static const char* smArchiveFilenames[];

	FsArchive(eType type);
	FsArchive(const string& filename);
	virtual ~FsArchive();

	void init();

	virtual int32_t loadFile(const string& filename, uint8_t*& buffer) const;
	virtual bool seekFile(const string& filename, FILE*& file, uint32_t& size) const;



private:
	string mFilename;
	FILE* mFile;

	uint32_t mItemCount;
	char* mItemList;


};

#endif /* __FSARCHIVE_H__ */
