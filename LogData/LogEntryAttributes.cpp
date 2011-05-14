/*
 * LogEntryAttributes.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogData/LogEntryAttributes.h"

#include <QString>

#include "LogData/LogEntryAttributeFactory.h"
#include "LogData/StringCache.h"

LogEntryAttributes::LogEntryAttributes( LogEntryAttributeFactory *factory, const std::vector<boost::shared_ptr<QString> > &defAttributes  )
	: attributes( defAttributes )
	, myFactory( factory )
{
}

LogEntryAttributes::~LogEntryAttributes() {
}

void LogEntryAttributes::setAttribute( boost::shared_ptr<QString> str, int idx )
{
	attributes[idx] = myFactory->getCache(idx).getString( str );
}

boost::shared_ptr<const QString> LogEntryAttributes::getAttribute( int idx ) const
{
	return attributes[idx];
}
