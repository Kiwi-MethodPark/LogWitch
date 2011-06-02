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

LogEntryAttributes::LogEntryAttributes( LogEntryAttributeFactory *factory, const std::vector<TSharedConstQString > &defAttributes  )
	: attributes( defAttributes )
	, myFactory( factory )
{
}

LogEntryAttributes::~LogEntryAttributes() {
}

void LogEntryAttributes::setAttribute( TSharedConstQString str, int idx )
{
	attributes[idx] = myFactory->getCache(idx).getString( str );
}

boost::shared_ptr<const QString> LogEntryAttributes::getAttribute( int idx ) const
{
	return attributes[idx];
}

const TSharedConstQString &LogEntryAttributes::operator []( int idx) const
{
    return attributes[idx];
}

