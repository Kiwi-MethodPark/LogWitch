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

/**
 * This class caches strings for usage. The class is a container
 * which gives a new string or a cached ptr to the already saved string.
 * This class ensures there is always one string instance for a specific
 * string present.
 */
class StringCache {
public:
	StringCache();

	boost::shared_ptr<QString> getString( boost::shared_ptr<QString> );

private:
	struct lessStringSharedPtr
	{
	  bool operator() (const boost::shared_ptr<QString> &s1, const boost::shared_ptr<QString> &s2) const
	  {return (*s1) < (*s2);}
	};

	typedef std::set< boost::shared_ptr<QString>, StringCache::lessStringSharedPtr > TMyCache;

	TMyCache cache;
};

#endif /* STRINGCACHE_H_ */
