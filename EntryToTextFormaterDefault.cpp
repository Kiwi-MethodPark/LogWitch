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

	int messageId = 2;

    for( int i = 0; i < entry->getAttributes().getFactory().getNumberOfFields(); i++ )
    {
        if( i != messageId )
            str << "<b>" << entry->getAttributes().getFactory().getDescription(i) << ":</b> " << *entry->getAttributes()[i] << "<br/>";
    }

    if( entry->getAttributes().getFactory().getNumberOfFields() > messageId)
        tmp = *entry->getAttributes()[messageId];

	str << "<b>Message:</b><p> <pre> " << tmp.replace("\n","<br/>") << "</pre></p>";

	return out;
}

