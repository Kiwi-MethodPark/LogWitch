/*
 * LogEntryAttributes.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogData/LogEntry.h"

#include <QString>

#include "LogData/ObjectCache.hxx"
#include "LogData/LogEntryFactory.h"

LogEntry::LogEntry( LogEntryFactory *factory, const std::vector<TSharedConstQString > &defAttributes  )
	: attributes( defAttributes )
	, myFactory( factory )
{
}

LogEntry::~LogEntry() {
}

void LogEntry::setAttribute( TSharedConstQString str, int idx )
{
	attributes[idx] = myFactory->getCache(idx).getObject( str );
}

boost::shared_ptr<const QString> LogEntry::getAttribute( int idx ) const
{
	return attributes[idx];
}

const TSharedConstQString &LogEntry::operator []( int idx) const
{
    return attributes[idx];
}

