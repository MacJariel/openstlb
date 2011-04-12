/*
 * BattleMapStructures.h
 *
 *  Created on: 5.4.2011
 *      Author: macjariel
 */

#ifndef __BATTLEMAPSTRUCTURES_H__
#define __BATTLEMAPSTRUCTURES_H__

#include "Setup/common.h"

namespace BattleMapSystem
{
class BattleMap;

class FilenameList
{
public:
	/**
	 * Returns the element at position n in the list..
	 * Throws std::out_of_range exception if such position does not exist in the list.
	 */
	string operator[](size_t n) const;

	/**
	 * Returns the number of elements in the list.
	 */
	inline uint32_t size() const
	{
		return mSize;
	}

private:
	friend class BattleMap;

	FilenameList();
	FilenameList(const FilenameList& other);
	const FilenameList& operator=(const FilenameList& other);

	/**
	 * Initializes the list.
	 * @param data Pointer to memory with the raw list, created with new char[] operator.
	 * @param size Number of sprite names in the list.
	 */
	void init(char* data, uint32_t size);

	void setExtension(const string& extension);

	/// Private destructor.
	~FilenameList();

	const char* mData;
	uint32_t mSize;
	string mExtension;
};

}

#endif /* __BATTLEMAPSTRUCTURES_H__ */
