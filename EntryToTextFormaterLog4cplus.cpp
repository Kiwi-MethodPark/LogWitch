/*
 * EntryToTextFormaterLog4cplus.cpp
 *
 *  Created on: Jun 2, 2011
 *      Author: sven
 */

#include "EntryToTextFormaterLog4cplus.h"

#include <QTextStream>

#include "LogData/LogEntry.h"
#include "LogData/LogEntryAttributeFactory.h"
#include "LogData/LogEntryAttributes.h"

QString EntryToTextFormaterLog4cplus::formatEntry( TconstSharedLogEntry entry ) const
{
    QString out;
    QTextStream str( &out );
    QString tmp;

    // retrieve the attributes from the factory ...
    if( entry->getAttributes().getFactory().getNumberOfFields() == 5 )
    {
        str << "<b>Timestamp:</b> " << *entry->getAttributes()[1] << "<br/>";
        str << "<b>Loglevel:</b> " << *entry->getAttributes()[3] << "<br/>";
        str << "<b>Component:</b> " << *entry->getAttributes()[4] << "<br/>";
        str << "<b>File source:</b> " << *entry->getAttributes()[5] << "<br/>";
        str << "<b>Thread:</b> " <<  *entry->getAttributes()[6]<< "<br/>";
        str << "<b>Nested diagnostic content:</b> " << *entry->getAttributes()[7] << "<br/>";
        tmp = *entry->getAttributes()[2];
        str << "<b>Message:</b><p> <pre> " << tmp.replace("\n","<br/>") << "</pre></p>";
    }
    else
        str << "<b>The attribute entry type seems to be incorrect! Please check.</b><br/>";


    return out;
}
