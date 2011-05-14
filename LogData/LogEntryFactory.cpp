/*
 * LogEntryFactory.cpp
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#include "LogEntryFactory.h"

LogEntryFactory::LogEntryFactory() {
	// TODO Auto-generated constructor stub

}

boost::shared_ptr<LogEntry> LogEntryFactory::generateLogEntry( const QDateTime &date, boost::shared_ptr<QString> source, boost::shared_ptr<QString> severity, const QString &message)
{
	severity = severityCache.getString( severity );
	source = sourceCache.getString( source );

	return boost::shared_ptr<LogEntry>( new LogEntry( date, source, severity, message ) );
}
