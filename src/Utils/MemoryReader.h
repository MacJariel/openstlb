/*
 * MemoryReader.h
 *
 *  Created on: 9.4.2011
 *      Author: macjariel
 */

#ifndef __MEMORYREADER_H__
#define __MEMORYREADER_H__

#include "IReader.h"

namespace Utils
{

class MemoryReader: public IReader
{
public:
	MemoryReader(const uint8_t* data, uint32_t size);
	virtual ~MemoryReader();

	/**
	 * Reads size bytes of data and stores it to buffer.
	 */
	virtual IReader& read(uint8_t* buffer, uint32_t size);

	/**
	 * Reads size bytes long number in little endian encoding and stores it to buffer.
	 */
	virtual IReader& readNumber(uint8_t* buffer, uint32_t size);

	/**
	 * Returns the position in the read object.
	 */
	virtual uint32_t pos() const;

	/**
	 * Sets the position in the read object.
	 */
	virtual void setPos(uint32_t newPos);

	/**
	 * Skips the next size bytes. Negative number skips backwards.
	 */
	virtual void skip(int32_t size);

	/**
	 * Returns the size of the read object.
	 */
	virtual uint32_t size() const;

private:
	const uint8_t* mData;
	uint32_t mSize;
	uint32_t mPos;
};

}
#endif /* __MEMORYREADER_H__ */
