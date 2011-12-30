/*
 * LogEntryParser_Logfile.cpp
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#include "LogEntryParser_Logfile.h"

#include <QRegExp>
#include <QtCore/QtCore>

#include "LogEntryFactory.h"
#include "LogData/LogEntryFactory.h"
#include "LogData/LogEntry.h"
#include "LogEntryParserModelConfiguration.h"
#include "LogEntryAttributeNames.h"

LogEntryParser_Logfile::LogEntryParser_Logfile( const QString &filename)
	: m_abort(false )
	, logfile( filename )
	, logfileStreamReady( false )
	, lineMessageRegex( new QRegExp("^([\\d-]+\\s+[\\d\\,\\:]+)\\s+-\\s+(.*)\\s+-\\s+\\[(.*)\\]\\s+-\\s+(.*)$") )
	, cellRegex( "\\s+-\\s+" )
	, timeFormat( "yyyy-MM-dd HH:mm:ss,zzz" )
    , myFactory( new LogEntryFactory )
	, m_Name( QFileInfo( filename ).fileName() )
    , m_logEntryNumber( 0 )
{
	lineMessageRegex->setMinimal(true);

	// Preparing attributes in factory
    LogEntryAttributeNames names;
    myFactory->addField(names.attDescNumber,false);
    myFactory->addField(names.attDescTimestamp,false);
    myFactory->addField(names.attDescMessage,false);

	myFactory->addField(names.attDescLoglevel, true);
	myFactory->addField(names.attDescLogger, true);
	myFactory->addField(names.attDescFileSource, true);
	myFactory->disallowAddingFields();

	m_myModelConfig = boost::shared_ptr<LogEntryParserModelConfiguration>( new LogEntryParserModelConfiguration("Logfile") );
	m_myModelConfig->setLogEntryFactory( myFactory );
	m_myModelConfig->setHierarchySplitString( 4, "\\.");

    for( int i = 0; i < myFactory->getNumberOfFields(); ++i )
    {
        m_myModelConfig->setFieldWidthHint( i,
                names.getDefautlForColumn( myFactory->getDescShort( i ) ).defaultCellWidth );
    }
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

boost::shared_ptr<LogEntryParserModelConfiguration> LogEntryParser_Logfile::getParserModelConfiguration() const
{
	return m_myModelConfig;
}

bool LogEntryParser_Logfile::initParser()
{
    if (!logfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_initError = tr("Unable to open file for reading.");
        return false;
    }

    logfileStream.setDevice( &logfile );
    logfileStreamReady = true;

    return true;
}

void LogEntryParser_Logfile::run()
{
	int i = 0;

	TSharedNewLogEntryMessage newEntryMessage( new NewLogEntryMessage );

	forever
	{
		if ( m_abort)
			return;

		TSharedLogEntry entry( getNextLogEntry() );

        if( entry )
            newEntryMessage->entries.push_back( entry );
        else
        	m_abort = true;

        i++;
	}

    emit newEntry( newEntryMessage );

	qDebug() << "We got " << i << " entries from logfile.";
}

TSharedLogEntry LogEntryParser_Logfile::getNextLogEntry()
{
	TSharedLogEntry entryReturn;

	if( logfileStreamReady )
	{
		bool entryComplete = false;

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
				//qDebug() << "StashedLine = " << stashedLine;
				int idx = -1;
				if( (idx = lineMessageRegex->indexIn( stashedLine )) != -1 )
				{
					if( entry ) // first entry
					{
						//qDebug() << "Appending Message to last entry = " << message;

						entry->setAttribute( QVariant( ++m_logEntryNumber ), 0 );
						entry->setAttribute( QVariant( QString(message) ), 2 );
						entryComplete = true;
						entryReturn = entry;
					}

					entry = myFactory->getNewLogEntry( );

					entry->setAttribute( QVariant(
					        QDateTime::fromString ( lineMessageRegex->cap(1), timeFormat ) ), 1 );
					// File Source
					entry->setAttribute( QVariant( lineMessageRegex->cap(3) ), 5 );
					message = lineMessageRegex->cap(4);

					QStringList lst = lineMessageRegex->cap(2).split( cellRegex );
					if( lst.size() >= 2 )
					{
						// Severity
						entry->setAttribute( QVariant( lst[0] ), 3 );
						// Component
						entry->setAttribute( QVariant( lst[1] ), 4 );
					}

					/*
					qDebug() << "Entry detected: timestamp = "<< timestamp
							<< " 0 " << lineMessageRegex->cap(0)
							<< " 1 " << lineMessageRegex->cap(1)
							<< " 2 " << lineMessageRegex->cap(2)
							<< " 3 " << lineMessageRegex->cap(3)
							<< " 4 " << lineMessageRegex->cap(4)
							<< " 5 " << lineMessageRegex->cap(5)
							<< " 6 " << lineMessageRegex->cap(6)
							<< " IDX: " << idx;
							*/

				}
				else
				{
					//qDebug() << " appending stashed line to message";
					message += "\n" + stashedLine;
				}

				stashedLine = "";
			}
		}
    }

    return entryReturn;
}
