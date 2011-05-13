/*
 * LzwCodec.cpp
 *
 *  Created on: 15.4.2011
 *      Author: macjariel
 */

#include "LzwCodec.h"
#include "StreamIO.h"

namespace
{
typedef unsigned char byte;

// Maps bytes in the input to the lowest possible values:
unsigned calculateByteMap(const std::vector<byte>& input, byte* byteMap)
{
	const size_t size = input.size();
	bool usedByte[256];

	for (unsigned i = 0; i < 256; ++i)
	{
		usedByte[i] = false;
		byteMap[i] = 255;
	}

	for (size_t i = 0; i < size; ++i)
		usedByte[input[i]] = true;

	unsigned byteMapCount = 0;
	for (unsigned i = 0; i < 256; ++i)
	{
		if (usedByte[i])
			byteMap[i] = byteMapCount++;
	}
	return byteMapCount;
}

// Calculates the minimum amount of bits required to store the specified
// value:
unsigned requiredBits(unsigned value)
{
	unsigned bits = 1;
	while ((value >>= 1) > 0)
		++bits;
	return bits;
}

// The string element:
struct CodeString
{
	unsigned prefixIndex;

	// First CodeString using this CodeString as prefix:
	unsigned first;

	// Next CodeStrings using the same prefixIndex as this one:
	unsigned nextLeft, nextRight;

	byte k;

	CodeString(byte newByte = 0, unsigned pI = ~0U) :
		prefixIndex(pI), first(~0U), nextLeft(~0U), nextRight(~0U), k(newByte)
	{
	}
};

class Dictionary
{
	std::vector<CodeString> table;
	unsigned codeStart, newCodeStringIndex;
	std::vector<byte> decodedString;

	// Returns ~0U if c didn't already exist, else the index to the
	// existing CodeString:
	unsigned add(CodeString& c)
	{
		if (c.prefixIndex == ~0U)
			return c.k;

		unsigned index = table[c.prefixIndex].first;
		if (index == ~0U)
			table[c.prefixIndex].first = newCodeStringIndex;
		else
		{
			while (true)
			{
				if (c.k == table[index].k)
					return index;
				if (c.k < table[index].k)
				{
					const unsigned next = table[index].nextLeft;
					if (next == ~0U)
					{
						table[index].nextLeft = newCodeStringIndex;
						break;
					}
					index = next;
				}
				else
				{
					const unsigned next = table[index].nextRight;
					if (next == ~0U)
					{
						table[index].nextRight = newCodeStringIndex;
						break;
					}
					index = next;
				}
			}
		}
		table[newCodeStringIndex++] = c;

		return ~0U;
	}

	void fillDecodedString(unsigned code)
	{
		decodedString.clear();
		while (code != ~0U)
		{
			const CodeString& cs = table[code];
			decodedString.push_back(cs.k);
			code = cs.prefixIndex;
		}
	}

public:
	Dictionary(unsigned maxBits, unsigned codeStart) :
		table(1 << maxBits), codeStart(codeStart), newCodeStringIndex(codeStart)
	{
		for (unsigned i = 0; i < codeStart; ++i)
			table[i].k = i;
	}

	bool searchCodeString(CodeString& c)
	{
		unsigned index = add(c);
		if (index != ~0U)
		{
			c.prefixIndex = index;
			return true;
		}
		return false;
	}

	void decode(unsigned oldCode, unsigned code, std::vector<byte>& outStream, const byte* byteMap)
	{
		const bool exists = code < newCodeStringIndex;

		if (exists)
			fillDecodedString(code);
		else
			fillDecodedString(oldCode);

		for (size_t i = decodedString.size(); i > 0;)
			outStream.push_back(byteMap[decodedString[--i]]);
		if (!exists)
			outStream.push_back(byteMap[decodedString.back()]);

		assert(newCodeStringIndex != oldCode);
		table[newCodeStringIndex].prefixIndex = oldCode;
		table[newCodeStringIndex++].k = decodedString.back();
	}

	unsigned size() const
	{
		return newCodeStringIndex;
	}

	void reset()
	{
		newCodeStringIndex = codeStart;
		for (unsigned i = 0; i < codeStart; ++i)
			table[i] = CodeString(i);
	}
};

void LZW_Encode(const std::vector<byte>& input, std::vector<byte>& encoded, unsigned maxBits)
{
	assert(maxBits < 32);
	const size_t size = input.size();
	encoded.clear();
	encoded.reserve(size + size / 8);

	byte byteMap[256];
	const unsigned byteMapSize = calculateByteMap(input, byteMap);
	const bool mapped = byteMapSize < 256;
	const unsigned eoiCode = byteMapSize;
	const unsigned codeStart = byteMapSize + 1;
	const unsigned minBits = requiredBits(codeStart);
	if (maxBits < minBits)
		maxBits = minBits;

	encoded.push_back(byte(maxBits));
	encoded.push_back(byte(byteMapSize));
	if (mapped)
		for (unsigned i = 0; i < 256; ++i)
			if (byteMap[i] < 255)
				encoded.push_back(byte(i));

	Dictionary dictionary(maxBits, codeStart);
	StreamWriter writer(encoded);
	CodeString currentString;
	unsigned currentBits = minBits;
	unsigned nextBitIncLimit = (1 << minBits) - 1;

	for (size_t i = 0; i < size; ++i)
	{
		currentString.k = byteMap[input[i]];
		if (!dictionary.searchCodeString(currentString))
		{
			writer.write(currentString.prefixIndex, currentBits);
			currentString.prefixIndex = currentString.k;

			if (dictionary.size() == nextBitIncLimit)
			{
				if (currentBits == maxBits)
				{
					currentBits = minBits;
					dictionary.reset();
				}
				else
					++currentBits;
				nextBitIncLimit = (1 << currentBits) - 1;
			}
		}
	}

	writer.write(currentString.prefixIndex, currentBits);
	if (dictionary.size() == nextBitIncLimit - 1)
		++currentBits;
	writer.write(eoiCode, currentBits);
}
}

namespace Utils
{

LzwCodec::LzwCodec()
{

}

LzwCodec::~LzwCodec()
{
}

void LzwCodec::encode(const vector<unsigned char> & input, std::vector<unsigned char> & encoded)
{
}

void LzwCodec::decode(const vector<unsigned char> & encoded, std::vector<unsigned char> & decoded)
{

	decoded.clear();

	byte byteMap[256];
	const unsigned maxBits = 16;
	const unsigned byteMapSize = (encoded[0] == 0 ? 256 : encoded[0]);
	const unsigned eoiCode = byteMapSize;
	const unsigned codeStart = byteMapSize + 1;
	const unsigned minBits = encoded[2];

	if (byteMapSize < 256)
		for (unsigned i = 0; i < byteMapSize; ++i)
			byteMap[i] = encoded[i + 3];
	else
		for (unsigned i = 0; i < 256; ++i)
			byteMap[i] = byte(i);

	Dictionary dictionary(maxBits, codeStart);
	StreamReader reader(encoded, encoded[0] == 0 ? 3 : 3 + byteMapSize);

	while (true)
	{
		dictionary.reset();
		unsigned currentBits = minBits;
		unsigned nextBitIncLimit = (1 << minBits) - 1;

		unsigned code = reader.read(currentBits);
		//printf("%3d, %d\n", currentBits, code);
		if (code == eoiCode)
			return;
		decoded.push_back(byteMap[code]);
		unsigned oldCode = code;

		while (true)
		{
			if (dictionary.size() >= nextBitIncLimit)
			{
				if (currentBits == maxBits)
					break;
				else
					++currentBits;
				nextBitIncLimit = (1 << currentBits) - 1;
			}

			code = reader.read(currentBits);
			//printf("%3d, %d\n", currentBits, code);
			if (code == eoiCode)
			{
				break;
			}

			dictionary.decode(oldCode, code, decoded, byteMap);
			/*
			if (dictionary.size() >= nextBitIncLimit)
			{
				if (currentBits == maxBits)
					break;
				else
					++currentBits;
				nextBitIncLimit = (1 << currentBits) - 1;
			}
			*/

			oldCode = code;
		}
	}
}
}
