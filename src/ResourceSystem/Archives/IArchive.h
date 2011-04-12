/*
 * IArchive.h
 *
 *  Created on: 4.4.2011
 *      Author: macjariel
 */

#ifndef __IARCHIVE_H__
#define __IARCHIVE_H__

class IArchive {
public:
	IArchive() {}
	virtual ~IArchive() {}

	virtual bool seekFile(const string& filename, FILE*& file, uint32_t& size) const = 0;

	/**
	 * Loads the file from the archive into memory.
	 * @param filename The name of the file in the archive.
	 * @param buffer Pointer to data (output parameter).
	 * @returns size of data, or -1 in case of error.
	 */
	virtual int32_t loadFile(const string& filename, uint8_t*& buffer) const = 0;

};

#endif /* __IARCHIVE_H__ */
