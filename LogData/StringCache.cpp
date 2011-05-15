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
	if( *it == str ) // str new to container, so invoke update request
		emit newStringElement( *it );

	return *it;
}


