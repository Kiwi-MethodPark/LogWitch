/*
 * LogEntryAttributes.h
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
class StringCache;
class LogEntryAttributes;

/**
 * This class holds a list of attributes which can be used by logfiles.
 * To prepare the class, first add all fields and then call disallowAddingFields.
 * Afterwards the class is ready for usage. Calling getDescription / getCache or
 * getNumberOfFields while not called disallowAddingFields before results in
 * an assertion.
 */
class LogEntryAttributeFactory {
public:
	LogEntryAttributeFactory();

	virtual ~LogEntryAttributeFactory();

	boost::shared_ptr<LogEntryAttributes> getNewLogEntryAttributes();

	/**
	 * Adds a field with a given description to the attributes list.
	 */
	void addField( const QString &description );

	/**
	 * Returns the total numbers of fields.
	 */
	int getNumberOfFields( ) const;

	/**
	 * Returns the description for a given field idx.
	 */
	const QString& getDescription( int idx ) const;

	/**
	 * Returns the correspinding string cache.
	 */
	StringCache& getCache( int idx );

	const StringCache& getCache( int idx ) const;

	/**
	 * Disallows adding fields and enables the usage of all getter
	 * methods.
	 */
	void disallowAddingFields();

private:
	std::vector<QString> fieldDescriptions;

	std::vector< boost::shared_ptr<StringCache> > fieldCaches;

	std::vector< TSharedConstQString > defaultLine;

	bool m_disallowAddingFields;
};

#endif /* LOGENTRYATTRIBUTES_H_ */
