/*
 * LogEntryParser_dummy.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogEntryParser_dummy.h"
#include "LogData/LogEntry.h"
#include "LogData/LogEntryAttributes.h"

LogEntryParser_dummy::LogEntryParser_dummy()
	: m_entries( 10 )
{
	// PReparing attributes factory
	myFactory.getLogEntryAttributeFactory()->addField("Severity");
	myFactory.getLogEntryAttributeFactory()->addField("Source");
	myFactory.getLogEntryAttributeFactory()->disallowAddingFields();
}

LogEntryParser_dummy::~LogEntryParser_dummy()
{
}

boost::shared_ptr<LogEntry> LogEntryParser_dummy::getNextLogEntry()
{
	boost::shared_ptr<LogEntry> entry;
	if( m_entries-- )
	{
		myFactory.generateLogEntry( QDateTime::currentDateTime(), "Message text :)");
		entry->getAttributes().setAttribute( boost::shared_ptr<QString>(new QString("ERROR")), 0 );
		entry->getAttributes().setAttribute( boost::shared_ptr<QString>(new QString("MainUnit.Logging")), 1 );
	}

	return entry;
}

const LogEntryAttributeFactory *LogEntryParser_dummy::getLogEntryAttributeFactory() const
{
	return myFactory.getLogEntryAttributeFactory();
}
