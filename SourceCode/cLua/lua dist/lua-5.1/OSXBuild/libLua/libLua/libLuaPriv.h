/*
 *  libLuaPriv.h
 *  libLua
 *
 *  Created by Jim Terhorst on 10/20/11.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

/* The classes below are not exported */
#pragma GCC visibility push(hidden)

class libLuaPriv
{
	public:
		void HelloWorldPriv(const char *);
};

#pragma GCC visibility pop
