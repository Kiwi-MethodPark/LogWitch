/*
 * LogEntryRemoveFilter.cpp
 *
 *  Created on: May 18, 2011
 *      Author: sven
 */

#include "LogEntryRemoveFilter.h"
#include "LogData/LogEntry.h"
#include "LogData/LogEntryAttributes.h"

LogEntryRemoveFilter::LogEntryRemoveFilter( int attr )
	: m_attributeID( attr )
{
}

LogEntryRemoveFilter::~LogEntryRemoveFilter()
{
}

void LogEntryRemoveFilter::addEntry( boost::shared_ptr<const QString> str )
{
	m_removeStrings.insert( str );
}

void LogEntryRemoveFilter::removeEntry( boost::shared_ptr<const QString> str )
{
	m_removeStrings.erase( str );
}

void LogEntryRemoveFilter::clear()
{
	m_removeStrings.clear();
}

bool LogEntryRemoveFilter::filterEntry( TconstSharedLogEntry entry ) const
{
	return m_removeStrings.end() == m_removeStrings.find( entry->getAttributes().getAttribute(m_attributeID) );
}
