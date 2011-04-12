/*
 * FsArchive.cpp
 *
 *  Created on: 4.4.2011
 *      Author: macjariel
 */

#include "FsArchive.h"
#include <cstdio>
#include <cstring>

const char* FsArchive::smArchiveFilenames[] =
{ "COMMON", "DEVAST", "MUSIC", "PUST", "RESEARCH", "SAMPLES", "T11", "TEXTS", 0 };

FsArchive::FsArchive(const string& filename) :
	mFile(0)
{
	mFilename = "DATA/" + filename + ".FS";
}

FsArchive::~FsArchive()
{
	// TODO Auto-generated destructor stub
}

void FsArchive::init()
{
	printf("Opening %s\n", mFilename.c_str());
	mFile = fopen(mFilename.c_str(), "rb");
	fread(&mItemCount, 4, 1, mFile);

	mItemList = (char*) malloc(mItemCount * (13 + 4 + 4));
	fread(mItemList, mItemCount * (13 + 4 + 4), 1, mFile);

	/*
	 for (int i = 0; i < mItemCount; ++i)
	 {
	 char itemName[13];
	 fread(itemName, 1, 13, mFile);
	 uint32_t pos, size;
	 fread(&pos, 4, 1, mFile);
	 fread(&size, 4, 1, mFile);
	 printf("%13s, %d, %d\n", itemName, pos, size);
	 }
	 */
}

int32_t FsArchive::loadFile(const string& filename, uint8_t*& buffer) const
{
	FILE* file;
	uint32_t size;
	if (!seekFile(filename, file, size))
	{
		return -1;
	}
	buffer = new uint8_t[size];
	fread(buffer, 1, size, file);
	return size;
}

bool FsArchive::seekFile(const string& name, FILE *& file, uint32_t & size) const
{
	const char* n = name.c_str();

	// TODO: use binary search or something better
	for (int i = 0; i < mItemCount; ++i)
	{
		if (strncmp(n, mItemList + i * 21, 13) == 0)
		{
			char* mItem = mItemList + i * 21;
			uint32_t pos;
			memcpy(&pos, mItem + 13, 4);
			memcpy(&size, mItem + 17, 4);
			//printf("Found %s, %d, %d\n", mItem, pos, size);
			fseek(mFile, pos, SEEK_SET);
			file = mFile;
			return true;
		}
	}
	printf("Resource %s not found.\n", name.c_str());
	return false;
}

