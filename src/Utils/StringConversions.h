/*
 * StringConversions.h
 *
 *  Created on: 10.4.2011
 *      Author: macjariel
 */

#ifndef __STRINGCONVERSIONS_H__
#define __STRINGCONVERSIONS_H__

#include "Setup/common.h"

namespace Utils
{
	namespace StringConversions
	{

		template<typename T>
		std::string toStdString(const T &thing)
		{
			std::ostringstream os;
			os << thing;
			return os.str();
		}

		template<typename T>
		T fromStdString(const string& str)
		{
			T result;
			std::istringstream is(str);
			if ((is >> result).fail())
			{
				return T();
			}
			return result;
		}

		inline char intToHexChar(uint8_t number)
		{
			number &= 15;
			if (number < 10)
				return (char)(number) + '0';
			else
				return ((char)(number) - 10) + 'A';
		}

		inline uint8_t hexCharToInt(char hexChar)
		{
			if (hexChar >= '0' && hexChar <= '9')
				return uint8_t(hexChar - '0');
			else if (hexChar >= 'A' && hexChar <= 'F')
				return uint8_t(hexChar - 'A') + 10;
			else
				return -1;
		}

	}
}
#endif /* __STRINGCONVERSIONS_H__ */
