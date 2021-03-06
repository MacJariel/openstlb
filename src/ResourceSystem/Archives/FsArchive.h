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

class FsArchive: public IArchive
{
public:

	static const char* smArchiveFilenames[];

	FsArchive(const string& filename);
	virtual ~FsArchive();

	virtual void init();

	virtual bool seekFile(const string& filename, FILE*& file, uint32_t& size) const;

	virtual int32_t loadFile(const string& filename, uint8_t*& buffer) const;

	virtual Utils::DataContainer loadFile(const string& filename) const;

private:
	string mFilename;
	FILE* mFile;

	uint32_t mItemCount;
	char* mItemList;

};

#endif /* __FSARCHIVE_H__ */
