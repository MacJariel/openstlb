/*
 * ResourceTypes.h
 *
 *  Created on: 4.4.2011
 *      Author: macjariel
 */

#ifndef __RESOURCETYPES_H__
#define __RESOURCETYPES_H__

namespace ResourceSystem
{
	enum eResourceType
	{
		RT_SPRITE = 0,

		NUM_RESTYPES
	};

	const char* const ResourceTypeNames[] =
	{
			"Sprite",

			"NumResTypes"
	};
}


#endif /* __RESOURCETYPES_H__ */
