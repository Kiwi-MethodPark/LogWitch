/*
 * StringCache.cpp
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#include "StringCache.h"

StringCache::StringCache()
{

}

boost::shared_ptr<QString> StringCache::getString( boost::shared_ptr<QString> str )
{
	TMyCache::iterator it = cache.insert( str ).first;
	if( *it == str ) // str already inside our container, so invoke update request
	{
		// TODO: We have to perform an update event / signal to listeners
	}

	return *it;
}


