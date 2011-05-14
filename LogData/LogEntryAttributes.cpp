/*
 * LogEntryAttributes.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogData/LogEntryAttributes.h"
#include <QString>

LogEntryAttributes::LogEntryAttributes( LogEntryAttributeFactory *factory, const std::vector<boost::shared_ptr<QString> > &defAttributes  )
	: attributes( defAttributes )
	, myFactory( factory )
{
}

LogEntryAttributes::~LogEntryAttributes() {
}
