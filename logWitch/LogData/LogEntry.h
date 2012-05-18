/*
 * LogEntry.h
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#ifndef LOGENTRYATTRIBUTES_H_
#define LOGENTRYATTRIBUTES_H_

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

#include <vector>

#include "Types.h"

class LogEntryFactory;
class QString;

class LogEntry
{
public:
	virtual ~LogEntry();

	LogEntry( LogEntryFactory *factory, const std::vector< QVariant > &defAttributes );

	/**
	 * Returns the factory which was used to generate this log entry.
	 * The factory also has more informations about the attributes and its
	 * interpretation.
	 */
	const LogEntryFactory &getFactory() const { return *myFactory; }

	/**
	 * Sets an attribute to the given value.
	 */
	void setAttribute( const QVariant &, int idx );

	/**
	 * Returns the raw value of the attribute
	 */
    const QVariant &getAttribute( int idx ) const;

	/**
	 * Returns the attribute as a string. If the attribute is not a string
	 * it will be converted to. (internally we user a cache)
	 */
    TSharedConstQString getAttributeAsString( int idx ) const;
	TSharedConstQString getAttributeAsString( int idx, boost::function<QString(const QVariant &)> customFormater ) const;

private:
	/**
	 *  These are our attributes. We use a raw pointer here, to save memory if
	 *  we have many objects of this class. Gives ~3%.
	 */
	QVariant *m_attributes;

	LogEntryFactory *myFactory;
};

typedef boost::shared_ptr<LogEntry> TSharedLogEntry;
typedef boost::shared_ptr<const LogEntry> TconstSharedLogEntry;

#endif /* LOGENTRYATTRIBUTES_H_ */
