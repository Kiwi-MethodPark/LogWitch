/*
 * LogEntryParser_Logfile.cpp
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#include "LogEntryParser_Logfile.h"

 #include <QRegExp>
#include "LogEntryFactory.h"
#include "LogData/LogEntryAttributeFactory.h"
#include "LogData/LogEntryAttributes.h"

LogEntryParser_Logfile::LogEntryParser_Logfile( const QString &filename)
	: m_abort(false )
	, logfile( filename )
	, logfileStreamReady( false )
	, lineMessageRegex( new QRegExp("^([\\d-]+)\\s+([\\d\\,\\:]+)\\s+([\\w]+)\\s+-\\s+(\".+\"|[\\S]+)\\s+-\\s\\[(.*)\\]\\s+-\\s+") )
	, timeFormat( "yyy-MM-dd HH:mm:ss,zzz" )
{
	lineMessageRegex->setMinimal(true);

	// PReparing attributes factory
	myFactory.getLogEntryAttributeFactory()->addField("Severity");
	myFactory.getLogEntryAttributeFactory()->addField("Component");
	myFactory.getLogEntryAttributeFactory()->addField("File source");
	myFactory.getLogEntryAttributeFactory()->disallowAddingFields();

}

LogEntryParser_Logfile::~LogEntryParser_Logfile()
{
	m_abort = true;
	wait();
}

void LogEntryParser_Logfile::startEmiting()
{
    if (!isRunning() && !m_abort )
        start(LowPriority);
}

boost::shared_ptr<const LogEntryAttributeFactory> LogEntryParser_Logfile::getLogEntryAttributeFactory() const
{
	return myFactory.getLogEntryAttributeFactory();
}

void LogEntryParser_Logfile::init()
{
    if (!logfile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    logfileStream.setDevice( &logfile );
    logfileStreamReady = true;
}

TSharedLogEntry LogEntryParser_Logfile::getNextLogEntry()
{
	TSharedLogEntry entryReturn;

	if( logfileStreamReady )
	{
		bool entryComplete = false;
		QString message;

		while( !entryComplete  )
		{
			if( stashedLine.isEmpty() && !logfileStream.atEnd() )
				stashedLine = logfileStream.readLine();

			if( logfileStream.atEnd() && stashedLine.isEmpty()  )
			{
				// End of logfile
				entryComplete = true;
			}
			else
			{
				if( lineMessageRegex->indexIn( stashedLine ) != -1 )
				{
					if( entry ) // first entry
					{
						entry->setMessage( message );
						entryComplete = true;
						entryReturn = entry;
					}

					QDateTime timestamp( QDateTime::fromString ( lineMessageRegex->cap(1), timeFormat ) );
					entry = myFactory.generateLogEntry( timestamp );
					entry->getAttributes().setAttribute( boost::shared_ptr<QString>(new QString(lineMessageRegex->cap(2)) ), 0 );
					entry->getAttributes().setAttribute( boost::shared_ptr<QString>(new QString(lineMessageRegex->cap(3)) ), 1 );
					entry->getAttributes().setAttribute( boost::shared_ptr<QString>(new QString(lineMessageRegex->cap(4)) ), 2 );
				}
				else
				{
					message += stashedLine;
				}
			}
		}
    }

    return entryReturn;
}
