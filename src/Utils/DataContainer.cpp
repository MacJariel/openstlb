/*
 * DataContainer.cpp
 *
 *  Created on: 19.4.2011
 *      Author: macjariel
 */

#include "DataContainer.h"

using namespace Utils;

DataContainer::DataContainer() :
	mData(nullptr), mSize(0)
{
}

DataContainer::DataContainer(uint8_t* data, uint32_t size) :
	mData(data), mSize(size)
{

}

DataContainer::DataContainer(DataContainer&& other):
mData(other.mData),
mSize(other.mSize)
{
	other.mData = nullptr;
	other.mSize = 0;
}

DataContainer::~DataContainer()
{
	delete[] mData;
}

const uint8_t& DataContainer::operator[](uint32_t index)const
{
	return mData[index];
}

 DataContainer& DataContainer::operator=(DataContainer&& other)
{
	cerr << "Using DataContainer move operator=" << endl;
	if (this != &other)
	{
		delete[] mData;
		mData = other.mData;
		mSize = other.mSize;

		other.mData = nullptr;
		other.mSize = 0;
	}
}

uint32_t DataContainer::size() const
{
	return mSize;
}
