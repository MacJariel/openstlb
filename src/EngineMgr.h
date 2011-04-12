/*
 * EngineMgr.h
 *
 *  Created on: 4.4.2011
 *      Author: macjariel
 */

#ifndef __ENGINEMGR_H__
#define __ENGINEMGR_H__

class EngineMgr {
public:
	EngineMgr();
	virtual ~EngineMgr();

	static EngineMgr& instance()
	{
		static EngineMgr engineMgr;
		return engineMgr;
	}

	bool startup();

	void shutdown();

};

#endif /* __ENGINEMGR_H__ */
