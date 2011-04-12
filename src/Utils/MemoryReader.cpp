/*
 * MemoryReader.cpp
 *
 *  Created on: 9.4.2011
 *      Author: macjariel
 */

#include "MemoryReader.h"

using namespace Utils;

MemoryReader::MemoryReader(const uint8_t* data, uint32_t size) :
	mData(data), mSize(size), mPos(0)
{
}

MemoryReader::~MemoryReader()
{
}

IReader& MemoryReader::read(uint8_t* buffer, uint32_t size)
{
	std::copy(mData + mPos, mData + mPos + size, buffer);
	mPos += size;
	return *this;
}

IReader& MemoryReader::readNumber(uint8_t* buffer, uint32_t size)
{
	std::copy(mData + mPos, mData + mPos + size, buffer);
	/// @TODO big endian platforms:
	/// std::reverse_copy(mData + mPos, mData + mPos + size, buffer);
	mPos += size;
	return *this;
}

uint32_t MemoryReader::pos() const
{
	return mPos;
}

void MemoryReader::setPos(uint32_t newPos)
{
	mPos = newPos;
}

void MemoryReader::skip(int32_t size)
{
	mPos += size;
}

uint32_t MemoryReader::size() const
{
	return mSize;
}
