/*
 * EntryToTextFormater.cpp
 *
 *  Created on: May 17, 2011
 *      Author: sven
 */

#include "EntryToTextFormater_Logfile.h"
#include "LogData/LogEntry.h"
#include <QTextStream>

QString EntryToTextFormater_Logfile::formatEntry( TconstSharedLogEntry entry ) const
{
	QString out;
	QTextStream str( &out );
	QString tmp;

	str << "<b>Timestamp:</b> " << entry->getTimestamp().toString( "dd.MM.yyyy hh:mm:ss.zzz" ) << "<br/>";
	tmp = entry->getMessage();
	str << "<b>Message:</b><p> <pre> " << tmp.replace("\n","<br/>") << "</pre></p>";

	return out;
}

