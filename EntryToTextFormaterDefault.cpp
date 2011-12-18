/*
 * EntryToTextFormater.cpp
 *
 *  Created on: May 17, 2011
 *      Author: sven
 */

#include "EntryToTextFormaterDefault.h"

#include <QTextStream>

#include "LogData/LogEntry.h"
#include "LogData/LogEntryFactory.h"

QString EntryToTextFormaterDefault::formatEntry( TconstSharedLogEntry entry ) const
{
	QString out;
	QTextStream str( &out );
	QString tmp;

	int messageId = 2;

    for( int i = 0; i < entry->getFactory().getNumberOfFields(); i++ )
    {
        if( i != messageId )
            str << "<b>" << entry->getFactory().getDescLong(i) << ":</b> " << *entry->getAttribute(i) << "<br/>";
    }

    if( entry->getFactory().getNumberOfFields() > messageId)
        tmp = *entry->getAttribute(messageId);

	str << "<b>Message:</b><p> <pre> " << tmp.replace("\n","<br/>") << "</pre></p>";

	return out;
}

