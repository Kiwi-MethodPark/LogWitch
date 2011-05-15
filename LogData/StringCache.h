/*
 * StringCache.h
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#ifndef STRINGCACHE_H_
#define STRINGCACHE_H_
#include <boost/shared_ptr.hpp>
#include <set>

#include <QString>
#include "Types.h"

/**
 * This class caches strings for usage. The class is a container
 * which gives a new string or a cached ptr to the already saved string.
 * This class ensures there is always one string instance for a specific
 * string present.
 */
class StringCache {
public:
	StringCache();

	TSharedConstQString getString( TSharedConstQString );

private:
	struct lessStringSharedPtr
	{
	  bool operator() (const TSharedConstQString &s1, const TSharedConstQString &s2) const
	  {return (*s1) < (*s2);}
	};

	typedef std::set< TSharedConstQString, StringCache::lessStringSharedPtr > TMyCache;

	TMyCache cache;
};

#endif /* STRINGCACHE_H_ */
