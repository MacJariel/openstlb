/*
 * BattleMapStructures.cpp
 *
 *  Created on: 5.4.2011
 *      Author: macjariel
 */

#include "BattleMapStructures.h"
#include <stdexcept>

using namespace BattleMapSystem;

FilenameList::FilenameList() :
	mData(0), mSize(0)
{
}

FilenameList::FilenameList(const FilenameList & other)
{
}

string FilenameList::operator[](size_t n) const
{
	if (n < mSize)
	{
		string result(mData + (n * 8), 8);
		size_t pos = result.find('\0');
		if (pos != string::npos)
		{
			result.erase(pos, result.size() - pos);
		}
		//printf("%d: %s\n", n, (result + mExtension).c_str());
		return result + mExtension;
	}
	throw std::out_of_range("Index out of range.");
}

void BattleMapSystem::FilenameList::setExtension(const string& extension)
{
	mExtension = extension;
}

FilenameList::~FilenameList()
{
	delete[] mData;
}

const FilenameList& FilenameList::operator=(const FilenameList& other)
{

}
void FilenameList::init(char* data, uint32_t size)
{
	mData = data;
	mSize = size;
}
