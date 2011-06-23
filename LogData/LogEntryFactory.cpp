/*
 * LogEntryFactory.cpp
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#include "LogEntryFactory.h"

#include "LogData/LogEntry.h"
#include "LogData/LogEntryAttributeFactory.h"


LogEntryFactory::LogEntryFactory()
	: m_indexedAttributesFactory( new LogEntryAttributeFactory )
{
}

TSharedLogEntry LogEntryFactory::generateLogEntry( )
{
	boost::shared_ptr<LogEntryAttributes> attr = m_indexedAttributesFactory->getNewLogEntryAttributes();

	return TSharedLogEntry( new LogEntry(attr) );
}

boost::shared_ptr<const LogEntryAttributeFactory> LogEntryFactory::getLogEntryAttributeFactory() const
{
	return m_indexedAttributesFactory;
}

boost::shared_ptr<LogEntryAttributeFactory> &LogEntryFactory::getLogEntryAttributeFactory()
{
	return m_indexedAttributesFactory;
}
