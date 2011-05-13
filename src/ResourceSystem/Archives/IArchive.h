/*
 * IArchive.h
 *
 *  Created on: 4.4.2011
 *      Author: macjariel
 */

#ifndef __IARCHIVE_H__
#define __IARCHIVE_H__

#include "Setup/common.h"
#include "Utils/DataContainer.h"

class IArchive {
public:
	IArchive() {}
	virtual ~IArchive() {}

	virtual void init() = 0;

	virtual bool seekFile(const string& filename, FILE*& file, uint32_t& size) const = 0;

	/**
	 * Loads the file from the archive into memory.
	 * @param filename The name of the file in the archive.
	 * @param buffer Pointer to data (output parameter).
	 * @deprecated You should use DataContainer loadFile(const string& filename) instead.
	 * @todo to be removed
	 * @returns size of data, or -1 in case of error.
	 */
	virtual int32_t loadFile(const string& filename, uint8_t*& buffer) const = 0;

	virtual Utils::DataContainer loadFile(const string& filename) const = 0;
};

#endif /* __IARCHIVE_H__ */
