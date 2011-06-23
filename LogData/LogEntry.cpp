/*
 * LogEntry.cpp
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#include "LogEntry.h"

LogEntry::LogEntry(const boost::shared_ptr<LogEntryAttributes> attr )
	: indexedLogEntries( attr )
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
