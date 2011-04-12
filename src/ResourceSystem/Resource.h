/*
 * Resource.h
 *
 *  Created on: 4.4.2011
 *      Author: macjariel
 */

#ifndef __RESOURCE_H__
#define __RESOURCE_H__

#include "Setup/common.h"

class Resource;
typedef Resource* ResourcePtr;

class IArchive;

class Resource {
public:
	enum eState
	{
		STATE_UNINITIALIZED=0,
		STATE_INITIALIED,
		STATE_LOADING,
		STATE_UNLOADING,
		STATE_LOADED
	};


	Resource();
	virtual ~Resource();

	bool load();

	bool unload();

	inline eState state() const { return mState; }

protected:

	virtual size_t loadImpl() = 0;

	virtual bool unloadImpl() = 0;

	inline void setState(eState state) { mState = state; }

	inline IArchive* archive() const { return mArchive; }

	inline const char* filename() const { return mFilename.c_str(); }

private:
	friend class ResourceMgr;
	inline void setArchive(IArchive* archive) { mArchive = archive; }
	inline void setFilename(const string& filename) { mFilename = filename; }

	eState mState;
	IArchive* mArchive;
	string mFilename;
};

#endif /* __RESOURCE_H__ */
