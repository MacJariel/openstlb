/*
 * common.h
 *
 *  Created on: 31.3.2011
 *      Author: macjariel
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ios>
#include <iterator>

#include "Setup/forwards.h"

using namespace std;

/**
 * nullptr workaround - TODO: detect if necessary
 */

const class
{
public:
	template<class T>
	operator T*() const
	{
		return 0;
	}
	template<class C, class T>
	operator T C::*() const
	{
		return 0;
	}
private:
	void operator&() const;
} nullptr =
{ };

#endif /* COMMON_H_ */
