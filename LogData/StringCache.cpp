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
	std::pair<TMyCache::iterator,bool> rv = cache.insert( str );
	if( rv.second ) // str new to container, so invoke update request
		emit newStringElement( *(rv.first) );

	return *(rv.first);
}


