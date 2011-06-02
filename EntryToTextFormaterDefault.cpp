/*
 * EntryToTextFormater.cpp
 *
 *  Created on: May 17, 2011
 *      Author: sven
 */

#include "EntryToTextFormaterDefault.h"

#include <QTextStream>

#include "LogData/LogEntry.h"
#include "LogData/LogEntryAttributeFactory.h"
#include "LogData/LogEntryAttributes.h"

QString EntryToTextFormaterDefault::formatEntry( TconstSharedLogEntry entry ) const
{
	QString out;
	QTextStream str( &out );
	QString tmp;

	str << "<b>Timestamp:</b> " << entry->getTimestamp().toString( "dd.MM.yyyy hh:mm:ss.zzz" ) << "<br/>";

    for( int i = 0; i < entry->getAttributes().getFactory().getNumberOfFields(); i++ )
    {
        str << "<b>" << entry->getAttributes().getFactory().getDescription(i) << ":</b> " << *entry->getAttributes()[i] << "<br/>";
    }

	tmp = entry->getMessage();
	str << "<b>Message:</b><p> <pre> " << tmp.replace("\n","<br/>") << "</pre></p>";

	return out;
}

