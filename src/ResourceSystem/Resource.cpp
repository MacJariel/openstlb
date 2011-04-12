/*
 * Resource.cpp
 *
 *  Created on: 4.4.2011
 *      Author: macjariel
 */

#include "Resource.h"

Resource::Resource() :
	mState(STATE_UNINITIALIZED) {
	// TODO Auto-generated constructor stub

}

Resource::~Resource() {
	// TODO Auto-generated destructor stub
}

bool Resource::load() {
	loadImpl();
}

bool Resource::unload() {
	unloadImpl();
}
