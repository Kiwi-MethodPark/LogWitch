/*
 * LogEntryFactory.h
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#ifndef LOGENTRYATTRIBUTEFACTORY_H_
#define LOGENTRYATTRIBUTEFACTORY_H_

#include <vector>
#include <boost/shared_ptr.hpp>
#include "Types.h"


class QString;
class ObjectCacheQStringSignaller;
template <class T> class GetObjectIF;
class LogEntry;

/**
 * This class holds a list of attributes which can be used by logfiles.
 * To prepare the class, first add all fields and then call disallowAddingFields.
 * Afterwards the class is ready for usage. Calling getDescription / getCache or
 * getNumberOfFields while not called disallowAddingFields before results in
 * an assertion.
 */
class LogEntryFactory {
public:
	LogEntryFactory();

	virtual ~LogEntryFactory();

	boost::shared_ptr<LogEntry> getNewLogEntry();

	/**
	 * Adds a field with a given description to the attributes list.
	 */
	void addField( const QString &descShort, const QString &descLong, bool cacheField );

	void addField( const std::pair<QString,QString> &desc, bool cacheField );

	/**
	 * Returns the total numbers of fields.
	 */
	int getNumberOfFields( ) const;

	/**
	 * Returns the description for a given field idx.
	 */
	const QString& getDescLong( int idx ) const;

	/**
	 * Returns the internal name, which is used for filter rules.
	 */
	const QString& getDescShort( int idx ) const;

	/**
	 * Returns the corresponding string cache.
	 */
	GetObjectIF<ObjectCacheQStringSignaller>& getCache( int idx );

	const GetObjectIF<ObjectCacheQStringSignaller>& getCache( int idx ) const;

	/**
	 * Disallows adding fields and enables the usage of all getter
	 * methods.
	 */
	void disallowAddingFields();

private:
	std::vector<std::pair<QString,QString> > fieldDescriptions;

	std::vector< boost::shared_ptr<GetObjectIF<ObjectCacheQStringSignaller> > > fieldCaches;

	std::vector< TSharedConstQString > defaultLine;

	bool m_disallowAddingFields;
};

#endif /* LOGENTRYATTRIBUTES_H_ */
