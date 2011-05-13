/*
 * FsuArchive.cpp
 *
 *  Created on: 15.4.2011
 *      Author: macjariel
 */

#include "FsuArchive.h"
#include "Utils/MemoryReader.h"

#include <cstring>

FsuArchive::FsuArchive(const string& filename) :
	mFilename(filename), mFile(nullptr), mDirectoryReader(nullptr)
{
}

FsuArchive::~FsuArchive()
{
	delete mDirectoryReader;
}

void FsuArchive::init()
{
	//printf("Opening %s\n", mFilename.c_str());
	mFile = fopen(mFilename.c_str(), "rb");
	if (mFile == 0) {
		cerr << "Cannot open archive " << mFilename << endl;
		return;
	}

	uint32_t directorySize;
	fread(&directorySize, 4, 1, mFile); // TODO: This is not endian-safe - rewrite using FileReader.
	uint8_t* directory = new uint8_t[directorySize];
	fread(directory, 1, directorySize, mFile);
	Utils::DataContainer dataContainer(directory, directorySize);

	mDirectoryReader = new Utils::MemoryReader(dataContainer, true);
}

int32_t FsuArchive::loadFile(const string& filename, uint8_t*& buffer) const
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

Utils::DataContainer FsuArchive::loadFile(const string& filename) const
{
	FILE* file;
	uint32_t size;
	if (!seekFile(filename, file, size))
	{
		return Utils::DataContainer();
	}
	uint8_t* buffer = new uint8_t[size];
	fread(buffer, 1, size, file);
	return Utils::DataContainer(buffer, size);
}

bool FsuArchive::seekFile(const string& filename, FILE *& file, uint32_t& size) const
{
	if (filename.size() != 8)
	{
		std::cerr << "Error while seeking " << filename << " in FSU archive. Filename must be 8 chars long." << endl;
		return false;
	}

	char buffer[8];
	const char* filename_cstr = filename.c_str();
	mDirectoryReader->setPos(0);
	uint32_t firstLevelSize = mDirectoryReader->readUint32();
	int32_t secondLevelPos = -1;

	for (int i = 0; i < firstLevelSize; ++i)
	{
		mDirectoryReader->read((uint8_t*)buffer, 5);
		if (strncmp(buffer, filename_cstr, 5) == 0)
		{
			secondLevelPos = mDirectoryReader->readInt32();
			break;
		}
		mDirectoryReader->skip(4);
	}

	if (secondLevelPos == -1)
		return false;

	mDirectoryReader->setPos(secondLevelPos - 4);
	uint32_t secondLevelSize = mDirectoryReader->readUint16();

	for (int i = 0; i < secondLevelSize; ++i)
	{
		mDirectoryReader->read((uint8_t*)buffer, 3);
		if (strncmp(buffer, filename_cstr + 5, 3) == 0)
		{
			int32_t filePos = mDirectoryReader->readInt32();
			size = mDirectoryReader->readInt16();
			fseek(mFile, filePos, SEEK_SET);
			file = mFile;
			return true;
		}
		mDirectoryReader->skip(6);
	}
	return false;
}
