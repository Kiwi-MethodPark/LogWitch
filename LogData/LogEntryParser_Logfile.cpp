/*
 * LogEntryParser_Logfile.cpp
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#include "LogEntryParser_Logfile.h"

 #include <QRegExp>

LogEntryParser_Logfile::LogEntryParser_Logfile( const QString &filename)
	: logfile( filename )
	, logfileStreamReady( false )
	, lineMessageRegex( new QRegExp("") )
{


}

void LogEntryParser_Logfile::init()
{
    if (!logfile.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    logfileStream.setDevice( &logfile );
    logfileStreamReady = true;
}

boost::shared_ptr<LogEntry> LogEntryParser_Logfile::getNextLogEntry()
{
	boost::shared_ptr<LogEntry> entry;

	if( logfileStreamReady )
	{
		bool entryComplete = false;
		QString message;

		while( !entryComplete  )
		{
			if( stashedLine.isEmpty() && !logfileStream.atEnd() )
				stashedLine = logfileStream.readLine();

			if( logfileStream.atEnd() && stashedLine.isEmpty() )
			{
				// End of logfile
				entryComplete = true;
			}
			else
			{
				// If signature of new line found, post last entry
				// to factory
				// Example logline:
				// 12.3.2011 14:23:12.456 ERROR MainUnit.Test.Nix [Test.cpp:23]: This is our log message
				//                Here comes more Text from a stacktrace for eg.
			}
		}
    }

    return entry;
}
