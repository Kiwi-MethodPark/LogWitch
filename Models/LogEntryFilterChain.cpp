/*
 * LogEntryFilterChain.cpp
 *
 *  Created on: May 19, 2011
 *      Author: sven
 */

#include "LogEntryFilterChain.h"

LogEntryFilterChain::LogEntryFilterChain()
{
}

LogEntryFilterChain::~LogEntryFilterChain()
{
}
void LogEntryFilterChain::addFilter( boost::shared_ptr<const LogEntryFilter> flt )
{
	m_filterChain.push_back( flt );
}

void LogEntryFilterChain::removeFilter( boost::shared_ptr<const LogEntryFilter>  flt )
{
	TFilterChain::iterator it = m_filterChain.begin();
	for( ; it != m_filterChain.end(); ++it )
	{
		if( *it == flt )
		{
			m_filterChain.erase( it );
			break;
		}
	}
}

bool LogEntryFilterChain::filterEntry( TconstSharedLogEntry entry ) const
{
	TFilterChain::const_iterator it = m_filterChain.begin();
	for( ; it != m_filterChain.end(); ++it )
	{
		if( !(*it)->filterEntry( entry ) )
			return false;
	}

	return true;
}
