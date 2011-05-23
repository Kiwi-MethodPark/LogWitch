/*
 * LogEntryParser_log4cplusSocket.cpp
 *
 *  Created on: May 23, 2011
 *      Author: sven
 */

#include "LogEntryParser_log4cplusSocket.h"

#include <boost/shared_ptr.hpp>

#include <log4cplus/socketappender.h>
#include <log4cplus/helpers/socket.h>
#include <log4cplus/spi/loggingevent.h>
#include <log4cplus/loglevel.h>

#include <QtCore/QtCore>

#include "EntryToTextFormater_Logfile.h"
#include "LogData/LogEntryAttributes.h"
#include "LogEntryAttributeFactory.h"
#include "LogEntryFactory.h"
#include "LogEntryParserModelConfiguration.h"


LogEntryParser_log4cplusSocket_ReceiverThread::LogEntryParser_log4cplusSocket_ReceiverThread(log4cplus::helpers::Socket sock, LogEntryParser_log4cplusSocket *parent)
: m_clientsock(sock)
, m_parent( parent )
{
	qDebug() << "Received a client connection!!!!";
}

LogEntryParser_log4cplusSocket_ReceiverThread::~LogEntryParser_log4cplusSocket_ReceiverThread()
{
	qDebug() << "Client connection closed.";
}

void LogEntryParser_log4cplusSocket_ReceiverThread::shutdownSocket()
{
	m_clientsock.close();
}

void LogEntryParser_log4cplusSocket_ReceiverThread::run()
{
	forever
	{
		if(!m_clientsock.isOpen())
			return;

		log4cplus::helpers::SocketBuffer msgSizeBuffer(sizeof(unsigned int));
		if(!m_clientsock.read(msgSizeBuffer))
			return;

		unsigned int msgSize = msgSizeBuffer.readInt();

		log4cplus::helpers::SocketBuffer buffer(msgSize);
		if(!m_clientsock.read(buffer))
			return;

		log4cplus::spi::InternalLoggingEvent event = readFromBuffer(buffer);

		QDateTime timestamp( QDateTime::fromMSecsSinceEpoch ( qint64(event.getTimestamp().getTime()) * 1000 + qint64( event.getTimestamp().usec() ) ) );

		TSharedLogEntry entry = m_parent->myFactory.generateLogEntry( timestamp );
		entry->setMessage( QString( event.getMessage().c_str() ) );

		boost::shared_ptr<QString> logLevel = m_parent->m_loglevelStringOff;
		if( event.getLogLevel() >= log4cplus::OFF_LOG_LEVEL )
			logLevel = m_parent->m_loglevelStringOff;
		else if( event.getLogLevel() >= log4cplus::FATAL_LOG_LEVEL )
			logLevel = m_parent->m_loglevelStringFatal;
		else if( event.getLogLevel() >= log4cplus::ERROR_LOG_LEVEL )
			logLevel = m_parent->m_loglevelStringError;
		else if( event.getLogLevel() >= log4cplus::WARN_LOG_LEVEL )
			logLevel = m_parent->m_loglevelStringWarn;
		else if( event.getLogLevel() >= log4cplus::DEBUG_LOG_LEVEL )
			logLevel = m_parent->m_loglevelStringDebug;
		else if( event.getLogLevel() >= log4cplus::TRACE_LOG_LEVEL )
			logLevel = m_parent->m_loglevelStringTrace;

		entry->getAttributes().setAttribute( logLevel , 0 );
		entry->getAttributes().setAttribute( TSharedConstQString(new QString( event.getLoggerName().c_str() ) ), 1 );
		entry->getAttributes().setAttribute( TSharedConstQString(new QString( event.getFile().c_str() )), 2 );

		m_parent->newEntry( entry );
	}
}


LogEntryParser_log4cplusSocket::LogEntryParser_log4cplusSocket( int port )
	:m_port(port)
{
	// Preparing attributes in factory
	myFactory.getLogEntryAttributeFactory()->addField("Severity");
	myFactory.getLogEntryAttributeFactory()->addField("Component");
	myFactory.getLogEntryAttributeFactory()->addField("File source");
	myFactory.getLogEntryAttributeFactory()->disallowAddingFields();

	m_myModelConfig = boost::shared_ptr<LogEntryParserModelConfiguration>( new LogEntryParserModelConfiguration );
	m_myModelConfig->setLogEntryAttributeFactory( myFactory.getLogEntryAttributeFactory() );
	m_myModelConfig->setHierarchySplitString( 1, "\\.");
	m_myModelConfig->setEntryToTextFormater( boost::shared_ptr<EntryToTextFormater>( new EntryToTextFormater_Logfile ) );

	m_myModelConfig->setFieldWidthHint( 0, 70 ); // severity
	m_myModelConfig->setFieldWidthHint( 1, 250 ); // component
	m_myModelConfig->setFieldWidthHint( 2, 150 ); // file source

	m_loglevelStringOff.reset(new QString("OFF"));
	m_loglevelStringFatal.reset(new QString("FATAL"));
	m_loglevelStringError.reset(new QString("ERROR"));
	m_loglevelStringWarn.reset(new QString("WARN"));
	m_loglevelStringDebug.reset(new QString("DEBUG"));
	m_loglevelStringTrace.reset(new QString("TRACE"));
}

LogEntryParser_log4cplusSocket::~LogEntryParser_log4cplusSocket()
{
	// First stop socket waiter
	m_abort = true;
	wait();

	// then stop receiver threads
	// m_myReceiverThreads
	while( !m_myReceiverThreads.empty() )
	{
		m_myReceiverThreads.front()->shutdownSocket();
		m_myReceiverThreads.front()->wait();
		delete m_myReceiverThreads.front();
		m_myReceiverThreads.pop_front();
	}
}

void LogEntryParser_log4cplusSocket::newEntryFromReceiver( TSharedLogEntry entry)
{
	emit newEntry( entry );
}

boost::shared_ptr<LogEntryParserModelConfiguration> LogEntryParser_log4cplusSocket::getParserModelConfiguration() const
{
	return m_myModelConfig;
}

void LogEntryParser_log4cplusSocket::startEmiting()
{
    if (!isRunning() && !m_abort )
        start(LowPriority);
}

void LogEntryParser_log4cplusSocket::run()
{
	log4cplus::helpers::ServerSocket serverSocket( m_port );

	if (!serverSocket.isOpen())
	{
		qDebug() << "Could not open server socket, maybe port "
			<< m_port << " is already in use.";
		return;
	}

	forever
	{
		LogEntryParser_log4cplusSocket_ReceiverThread *th = new LogEntryParser_log4cplusSocket_ReceiverThread( serverSocket.accept(), this );
		m_myReceiverThreads.push_back( th );
		th->start();
	}
}
