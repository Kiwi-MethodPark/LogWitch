/*
 * LogEntry.cpp
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#include "LogEntry.h"

LogEntry::LogEntry(const QDateTime &date, boost::shared_ptr<LogEntryAttributes> attr, const QString &message )
	: timestamp( date )
	, indexedLogEntries( attr )
	, message( message )
{

}

const LogEntryAttributes &LogEntry::getAttributes() const
{
	return *indexedLogEntries;
}

LogEntryAttributes &LogEntry::getAttributes()
{
	return *indexedLogEntries;
}

const QDateTime &LogEntry::getTimestamp() const
{
	return timestamp;
}

const QString &LogEntry::getMessage() const
{
	return message;
}
