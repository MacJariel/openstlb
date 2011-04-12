/*
 * ResourceMgr.cpp
 *
 *  Created on: 4.4.2011
 *      Author: macjariel
 */

#include "ResourceMgr.h"
#include "ResourceTypes.h"

// Resource types
#include "GfxSystem/Sprite.h"

using namespace ResourceSystem;

ResourceMgr::ResourceMgr() {

	mResourceCreationMethods[RT_SPRITE] = &Sprite::create;



}

ResourceMgr::~ResourceMgr() {
	// TODO Auto-generated destructor stub
}

ResourceId ResourceMgr::loadResource(const string& resourceArchive, const string& resourceName)
{


}
