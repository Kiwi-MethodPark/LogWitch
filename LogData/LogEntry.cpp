/*
 * LogEntry.cpp
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#include "LogEntry.h"

LogEntry::LogEntry(const QDateTime &date, boost::shared_ptr<QString> source, boost::shared_ptr<QString> severity, const QString &message )
	: timestamp( date )
	, source( source )
	, severity( severity )
	, message( message )
{

}
