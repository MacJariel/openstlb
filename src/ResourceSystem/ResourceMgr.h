/*
 * ResourceMgr.h
 *
 *  Created on: 4.4.2011
 *      Author: macjariel
 */

#ifndef __RESOURCEMGR_H__
#define __RESOURCEMGR_H__

#include "Resource.h"
#include "ResourceTypes.h"

namespace ResourceSystem {

typedef uint32_t ResourceId;

class ResourceMgr {
public:
	ResourceMgr();
	virtual ~ResourceMgr();

	ResourceId loadResource(const string& resourceArchive,
			const string& resourceName);
	void unloadResource(ResourceId resourceId);

private:
	typedef ResourcePtr (*ResourceCreationMethod)();
	ResourceCreationMethod mResourceCreationMethods[NUM_RESTYPES];

};
}
#endif /* __RESOURCEMGR_H__ */
