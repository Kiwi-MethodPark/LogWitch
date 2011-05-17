/*
 * EntryToTextFormater.cpp
 *
 *  Created on: May 17, 2011
 *      Author: sven
 */

#include "EntryToTextFormater.h"
#include "LogData/LogEntry.h"
#include <QTextStream>

EntryToTextFormater::EntryToTextFormater() {
	// TODO Auto-generated constructor stub

}

EntryToTextFormater::~EntryToTextFormater() {
	// TODO Auto-generated destructor stub
}

QString EntryToTextFormater::formatEntry( TconstSharedLogEntry entry ) const
{
	QString out;
	QTextStream str( &out );
	QString tmp;

	str << "<b>Timestamp:</b> " << entry->getTimestamp().toString( "dd.MM.yyyy hh:mm:ss.zzz" ) << "<br/>";
	tmp = entry->getMessage();
	str << "<b>Message:</b> " << tmp.replace("\n","<br/>") << "<br/>";

	return out;
}

