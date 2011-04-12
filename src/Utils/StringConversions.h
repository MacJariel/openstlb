/*
 * StringConversions.h
 *
 *  Created on: 10.4.2011
 *      Author: macjariel
 */

#ifndef __STRINGCONVERSIONS_H__
#define __STRINGCONVERSIONS_H__

#include "Setup/common.h"

template<typename T>
std::string toStdString(const T &thing)
{
	std::ostringstream os;
	os << thing;
	return os.str();
}

#endif /* __STRINGCONVERSIONS_H__ */
