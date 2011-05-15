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

TSharedConstQString StringCache::getString( TSharedConstQString str )
{
	TMyCache::iterator it = cache.insert( str ).first;
	if( *it == str ) // str already inside our container, so invoke update request
	{
		// TODO: We have to perform an update event / signal to listeners
	}

	return *it;
}


