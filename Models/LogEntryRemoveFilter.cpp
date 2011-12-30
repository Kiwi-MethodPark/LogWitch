/*
 * LogEntryRemoveFilter.cpp
 *
 *  Created on: May 18, 2011
 *      Author: sven
 */

#include "LogEntryRemoveFilter.h"
#include "LogData/LogEntry.h"

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
    Q_ASSERT( entry );
	return m_removeStrings.end() == m_removeStrings.find( entry->getAttributeAsString(m_attributeID) );
}
