/*
 * FsuArchive.h
 *
 *  Created on: 15.4.2011
 *      Author: macjariel
 */

#ifndef __FSUARCHIVE_H__
#define __FSUARCHIVE_H__

#include "Setup/common.h"
#include "IArchive.h"

class FsuArchive: public IArchive
{
public:
	FsuArchive(const string& filename);
	virtual ~FsuArchive();

	virtual void init();

	virtual bool seekFile(const string& filename, FILE*& file, uint32_t& size) const;

	virtual int32_t loadFile(const string& filename, uint8_t*& buffer) const;

	virtual Utils::DataContainer loadFile(const string& filename) const;

private:
	string mFilename;
	FILE* mFile;

	Utils::MemoryReader* mDirectoryReader;
};

#endif /* __FSUARCHIVE_H__ */
