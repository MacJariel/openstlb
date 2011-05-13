/*
 * MemoryReader.cpp
 *
 *  Created on: 9.4.2011
 *      Author: macjariel
 */

#include "MemoryReader.h"
#include "Utils/DataContainer.h"

using namespace Utils;

MemoryReader::MemoryReader(const DataContainer& data):
		mPos(0)
{
	mData = &const_cast<DataContainer&>(data);
	mOwnsData = false;
}

MemoryReader::MemoryReader(DataContainer&& data, bool moveContainer):
		mPos(0)
{
	assert(moveContainer == true);
	mData = new DataContainer(move(data));
	mOwnsData = true;
}

MemoryReader::~MemoryReader()
{
	if (mOwnsData)
		delete mData;
}

IReader& MemoryReader::read(uint8_t* buffer, uint32_t size)
{
	std::copy(&(*mData)[mPos], &(*mData)[mPos + size], buffer);
	mPos += size;
	return *this;
}

Utils::DataContainer MemoryReader::read(uint32_t size)
{
	uint8_t* buffer = new uint8_t[size];
	std::copy(&(*mData)[mPos], &(*mData)[mPos + size], buffer);
	return Utils::DataContainer(buffer, size);
}

IReader& MemoryReader::readNumber(uint8_t* buffer, uint32_t size)
{
	std::copy(&(*mData)[mPos], &(*mData)[mPos + size], buffer);
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
	return mData->size();
}
