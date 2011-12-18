/*
 * LogEntryParser_dummy.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogEntryParser_dummy.h"
#include "LogData/LogEntry.h"
#include "LogData/LogEntryFactory.h"
#include "LogEntryAttributeNames.h"

LogEntryParser_dummy::LogEntryParser_dummy()
	: m_entries( 0 )
	, m_abort( false )
	, m_count( 12 )
    , myFactory( new LogEntryFactory )
{
	// Preparing attributes factory
    LogEntryAttributeNames names;
    myFactory->addField(names.attDescNumber,false);
    myFactory->addField(names.attDescTimestamp,false);
    myFactory->addField(names.attDescMessage,false);

	myFactory->addField(names.attDescLoglevel,true);
	myFactory->addField(names.attDescLogger,true);
	myFactory->disallowAddingFields();

	m_myModelConfig = boost::shared_ptr<LogEntryParserModelConfiguration>( new LogEntryParserModelConfiguration("DummyLogger") );
	m_myModelConfig->setLogEntryFactory( myFactory );
	m_myModelConfig->setHierarchySplitString( 4, "\\.");

    m_myModelConfig->setFieldWidthHint( 0, 60 ); // number
    m_myModelConfig->setFieldWidthHint( 1, 180 ); // timestamp
    m_myModelConfig->setFieldWidthHint( 2, 500 ); // message
}

LogEntryParser_dummy::~LogEntryParser_dummy()
{
	m_abort = true;
	wait();
}

void LogEntryParser_dummy::addEntries( int count )
{
	m_count = count;
}

void LogEntryParser_dummy::startEmiting()
{
    if (!isRunning() && !m_abort )
        start(LowPriority);
}

void LogEntryParser_dummy::run()
{
	forever
	{
		for( int i = 0; i < 4; i++ )
		{
			usleep(250000);

			if( m_count )
				break;

			if ( m_abort)
				return;
		}

		TSharedNewLogEntryMessage newEntryMessage( new NewLogEntryMessage);

        while( m_count )
        {
            TSharedLogEntry entry( getNextLogEntry() );

            if( entry )
                newEntryMessage->entries.push_back( entry );

            m_count--;
            if( m_count < 0 )
                m_count = 0;
        }


		TSharedLogEntry entry( getNextLogEntry() );

        if( entry )
            newEntryMessage->entries.push_back( entry );

        if( !newEntryMessage->entries.empty() )
            emit newEntry( newEntryMessage );
        else
        	m_abort = true;
	}
}

bool LogEntryParser_dummy::initParser()
{
    static int inits = 0;

    if( (++inits % 3 ) == 0 )
    {
        m_initError = tr("Parser initialization failed (just a test)");
        return false;

    }
    return true;
}

TSharedLogEntry LogEntryParser_dummy::getNextLogEntry()
{
	TSharedLogEntry entry;
	//if( m_entries++ < 100000 )
	{
		m_entries++;

		if( m_entries == 20 )
		{
		    qDebug() << "emmiting error!";
		    emit signalError(QString("This is a generated error from dummy parser!"));
		}

		entry = myFactory->getNewLogEntry( );

		entry->setAttribute( boost::shared_ptr<QString>(new QString(QString("%1").arg(m_entries)) ), 0 );
		entry->setAttribute( boost::shared_ptr<QString>(new QString(QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss.zzz")) ), 1 );
		QString message( QString("Message #").append(QString("%1").arg(m_entries) ) );
		entry->setAttribute( boost::shared_ptr<QString>(new QString(message) ), 2 );

		entry->setAttribute( boost::shared_ptr<QString>(new QString("DEBUG") ), 3 );
		if( (m_entries % 20) == 0 )
			entry->setAttribute( boost::shared_ptr<QString>(new QString("WARN") ), 3 );
		if( (m_entries % 100) == 0 )
			entry->setAttribute( boost::shared_ptr<QString>(new QString("ERROR") ), 3 );


		entry->setAttribute( boost::shared_ptr<QString>(new QString("MainUnit.Logging")), 4 );
		if( (m_entries % 3) == 0 )
			entry->setAttribute( boost::shared_ptr<QString>(new QString("MainUnit.Logging1")), 4 );
		if( (m_entries % 10) == 0 )
			entry->setAttribute( boost::shared_ptr<QString>(new QString("MainUnit.Logging2")), 4 );
		if( (m_entries % 15) == 0 )
		    entry->setAttribute( boost::shared_ptr<QString>(new QString("MainUnit5.Logging3")), 4 );
		if( (m_entries % 20) == 0 )
			entry->setAttribute( boost::shared_ptr<QString>(new QString("MainUnit3.Logging4")), 4 );
		if( (m_entries % 21) == 0 )
			entry->setAttribute( boost::shared_ptr<QString>(new QString("Aplication2.Logging5")), 4 );
		if( (m_entries % 25) == 0 )
			entry->setAttribute( boost::shared_ptr<QString>(new QString("Aplication.Comp1")), 4 );
		if( (m_entries % 30) == 0 )
			entry->setAttribute( boost::shared_ptr<QString>(new QString("Aplication1.XTray.Loop")), 4 );

	}

	return entry;
}

boost::shared_ptr<LogEntryParserModelConfiguration> LogEntryParser_dummy::getParserModelConfiguration() const
{
	return m_myModelConfig;
}
