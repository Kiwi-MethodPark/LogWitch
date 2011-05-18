/*
 * LogEntryRemoveFilter.cpp
 *
 *  Created on: May 18, 2011
 *      Author: sven
 */

#include "LogEntryRemoveFilter.h"

LogEntryRemoveFilter::LogEntryRemoveFilter()
{
}

LogEntryRemoveFilter::~LogEntryRemoveFilter()
{
}

bool LogEntryRemoveFilter::filterEntry( TconstSharedLogEntry entry ) const
{
	return true;
}
