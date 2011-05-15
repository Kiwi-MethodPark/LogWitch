/*
 * LogEntryParser_dummy.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogEntryParser_dummy.h"
#include "LogData/LogEntry.h"
#include "LogData/LogEntryAttributes.h"
#include "LogData/LogEntryAttributeFactory.h"

LogEntryParser_dummy::LogEntryParser_dummy()
	: m_entries( 0 )
	, m_abort( false )
	, m_count( 12 )
{
	// PReparing attributes factory
	myFactory.getLogEntryAttributeFactory()->addField("Severity");
	myFactory.getLogEntryAttributeFactory()->addField("Source");
	myFactory.getLogEntryAttributeFactory()->disallowAddingFields();
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

		while( m_count )
		{
			TSharedLogEntry entry( getNextLogEntry() );

	        if( entry )
	        	emit newEntry( entry );
	        m_count--;
			if( m_count < 0 )
				m_count = 0;
		}

		TSharedLogEntry entry( getNextLogEntry() );

        if( entry )
        	emit newEntry( entry );
        else
        	m_abort = true;
	}
}

TSharedLogEntry LogEntryParser_dummy::getNextLogEntry()
{
	TSharedLogEntry entry;
	//if( m_entries++ < 100000 )
	{
		m_entries++;

		entry = myFactory.generateLogEntry( QDateTime::currentDateTime(), QString("Message #").append(QString("%1").arg(m_entries))  );
		entry->getAttributes().setAttribute( boost::shared_ptr<QString>(new QString("DEBUG") ), 0 );
		if( (m_entries % 20) == 0 )
			entry->getAttributes().setAttribute( boost::shared_ptr<QString>(new QString("WARN") ), 0 );
		if( (m_entries % 100) == 0 )
			entry->getAttributes().setAttribute( boost::shared_ptr<QString>(new QString("ERROR") ), 0 );


		entry->getAttributes().setAttribute( boost::shared_ptr<QString>(new QString("MainUnit.Logging")), 1 );
		if( (m_entries % 3) == 0 )
			entry->getAttributes().setAttribute( boost::shared_ptr<QString>(new QString("MainUnit.Logging1")), 1 );
		if( (m_entries % 10) == 0 )
			entry->getAttributes().setAttribute( boost::shared_ptr<QString>(new QString("MainUnit.Logging2")), 1 );
		if( (m_entries % 15) == 0 )
		entry->getAttributes().setAttribute( boost::shared_ptr<QString>(new QString("MainUnit5.Logging3")), 1 );
		if( (m_entries % 20) == 0 )
			entry->getAttributes().setAttribute( boost::shared_ptr<QString>(new QString("MainUnit3.Logging4")), 1 );
		if( (m_entries % 21) == 0 )
			entry->getAttributes().setAttribute( boost::shared_ptr<QString>(new QString("Aplication2.Logging5")), 1 );
		if( (m_entries % 25) == 0 )
			entry->getAttributes().setAttribute( boost::shared_ptr<QString>(new QString("Aplication.Comp1")), 1 );
		if( (m_entries % 30) == 0 )
			entry->getAttributes().setAttribute( boost::shared_ptr<QString>(new QString("Aplication1.XTray.Loop")), 1 );

	}

	return entry;
}

boost::shared_ptr<const LogEntryAttributeFactory> LogEntryParser_dummy::getLogEntryAttributeFactory() const
{
	return myFactory.getLogEntryAttributeFactory();
}
