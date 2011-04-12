/*
 * IReader.h
 *
 *  Created on: 9.4.2011
 *      Author: macjariel
 */

#ifndef __IREADER_H__
#define __IREADER_H__

#include "Setup/common.h"

namespace Utils
{

class IReader
{
public:
	IReader()
	{
	}

	virtual ~IReader()
	{
	}

	/**
	 * Reads size bytes of data and stores it to buffer.
	 */
	virtual IReader& read(uint8_t* buffer, uint32_t size) = 0;

	/**
	 * Reads size bytes long number in little endian encoding and stores it to buffer.
	 */
	virtual IReader& readNumber(uint8_t* buffer, uint32_t size) = 0;

	/**
	 * Returns the position in the read object.
	 */
	virtual uint32_t pos() const = 0;

	/**
	 * Sets the position in the read object.
	 */
	virtual void setPos(uint32_t newPos) = 0;

	/**
	 * Skips the next size bytes. Negative number skips backwards.
	 */
	virtual void skip(int32_t size) = 0;

	/**
	 * Returns the size of the read object.
	 */
	virtual uint32_t size() const = 0;

	inline uint8_t readUint8()
	{
		uint8_t res;
		readNumber(&res, 1);
		return res;
	}

	inline int8_t readInt8()
	{
		int8_t res;
		readNumber((uint8_t*) &res, 1);
		return res;
	}

	inline uint16_t readUint16()
	{
		uint16_t res;
		readNumber((uint8_t*) &res, 2);
		return res;
	}

	inline int16_t readInt16()
	{
		int16_t res;
		readNumber((uint8_t*) &res, 2);
		return res;
	}

	inline uint32_t readUint32()
	{
		uint32_t res;
		readNumber((uint8_t*) &res, 4);
		return res;
	}

	inline int32_t readInt32()
	{
		int32_t res;
		readNumber((uint8_t*) &res, 4);
		return res;
	}

};

}

#endif /* __IREADER_H__ */
