/*
 * LzwCodec.h
 *
 *  Created on: 15.4.2011
 *      Author: macjariel
 */

#ifndef __LZWCODEC_H__
#define __LZWCODEC_H__

#include "Setup/common.h"

namespace Utils
{

class LzwCodec
{
public:
	LzwCodec();

	~LzwCodec();

	void encode(const vector<unsigned char>& input, std::vector<unsigned char>& encoded);

	void decode(const vector<unsigned char>& encoded, std::vector<unsigned char>& decoded);
};

}

#endif /* __LZWCODEC_H__ */
