/*
 * LogEntryFactory.cpp
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#include "LogEntryFactory.h"


LogEntryFactory::LogEntryFactory() {


}

boost::shared_ptr<LogEntry> LogEntryFactory::generateLogEntry( const QDateTime &date, const QString &message)
{
	boost::shared_ptr<LogEntryAttributes> attr = m_indexedAttributesFactory.getNewLogEntryAttributes();

	return boost::shared_ptr<LogEntry>( new LogEntry( date, attr, message ) );
}
