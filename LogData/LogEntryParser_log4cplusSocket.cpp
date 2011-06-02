/*
 * LogEntryParser_log4cplusSocket.cpp
 *
 *  Created on: May 23, 2011
 *      Author: sven
 */

#include "LogData/LogEntryParser_log4cplusSocket.h"

#include <algorithm>
#include <boost/shared_ptr.hpp>
#include <iostream>

#include <log4cplus/socketappender.h>
#include <log4cplus/spi/loggingevent.h>
#include <log4cplus/loglevel.h>

#include <QtCore/QtCore>

#include "EntryToTextFormaterLog4cplus.h"
#include "LogData/LogEntryAttributes.h"
#include "LogEntryAttributeFactory.h"
#include "LogEntryFactory.h"
#include "LogEntryParserModelConfiguration.h"


LogEntryParser_log4cplusSocket::LogEntryParser_log4cplusSocket( int port )
	:m_port(port)
	,m_name( "Log4cplus Listener Port " + QString::number(port))
{
	// Preparing attributes in factory
	myFactory.getLogEntryAttributeFactory()->addField("Loglevel");
	myFactory.getLogEntryAttributeFactory()->addField("Component");
	myFactory.getLogEntryAttributeFactory()->addField("File source");
	myFactory.getLogEntryAttributeFactory()->addField("Thread");
	myFactory.getLogEntryAttributeFactory()->addField("NDC");
	myFactory.getLogEntryAttributeFactory()->disallowAddingFields();

	m_myModelConfig = boost::shared_ptr<LogEntryParserModelConfiguration>( new LogEntryParserModelConfiguration );
	m_myModelConfig->setLogEntryAttributeFactory( myFactory.getLogEntryAttributeFactory() );
	m_myModelConfig->setHierarchySplitString( 1, "\\.");
	m_myModelConfig->setEntryToTextFormater( boost::shared_ptr<EntryToTextFormater>( new EntryToTextFormaterLog4cplus ) );

	m_myModelConfig->setFieldWidthHint( 0, 70 ); // severity
	m_myModelConfig->setFieldWidthHint( 1, 250 ); // component
	m_myModelConfig->setFieldWidthHint( 2, 150 ); // file source
	m_myModelConfig->setFieldWidthHint( 3, 70 ); // thread
	m_myModelConfig->setFieldWidthHint( 4, 100 ); // NDC

	m_loglevelStringOff.reset(new QString("OFF"));
	m_loglevelStringFatal.reset(new QString("FATAL"));
	m_loglevelStringError.reset(new QString("ERROR"));
	m_loglevelStringWarn.reset(new QString("WARN"));
	m_loglevelStringDebug.reset(new QString("DEBUG"));
	m_loglevelStringTrace.reset(new QString("TRACE"));

	connect( this, SIGNAL(newConnection()), this, SLOT(newIncomingConnection()));
}

LogEntryParser_log4cplusSocket::~LogEntryParser_log4cplusSocket()
{
	qDebug() << "called: ~LogEntryParser_log4cplusSocket";
	close();
	qDebug() << "finished: ~LogEntryParser_log4cplusSocket";
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
	qDebug() << "Server listening on port: " << m_port;
	if( !listen ( QHostAddress::Any, m_port ) )
	{
	    emit signalError( "Listening on port " +QString::number( m_port) + " failed: "+ errorString() );
	}
}

void LogEntryParser_log4cplusSocket::newIncomingConnection()
{
	qDebug() << "Incoming connection ... creating new receiver.";

	QTcpSocket *socket = nextPendingConnection();
	LogEntryParser_log4cplusSocket_Receiver *receiver = new LogEntryParser_log4cplusSocket_Receiver( this, socket );

    connect(this, SIGNAL(destroyed()), receiver, SLOT(shutdown()));
    connect(receiver, SIGNAL(newEntry(TSharedLogEntry)), this, SLOT(newEntryFromReceiver(TSharedLogEntry)));
}

QString LogEntryParser_log4cplusSocket::getName() const
{
	return m_name;
}

LogEntryParser_log4cplusSocket_Receiver::LogEntryParser_log4cplusSocket_Receiver( LogEntryParser_log4cplusSocket *server, QTcpSocket *socket )
	: m_socket( socket )
	, m_stateReadSize( true )
	, m_server(server)
{
	qDebug() << "new receiver created";
	m_socket->setParent( this );
	connect( m_socket, SIGNAL(readyRead()), this, SLOT(newDataAvailable()));
	connect( m_socket, SIGNAL(disconnected()), this, SLOT(shutdown()));
}

LogEntryParser_log4cplusSocket_Receiver::~LogEntryParser_log4cplusSocket_Receiver()
{
	qDebug() << "receiver destroyed";
}

void LogEntryParser_log4cplusSocket_Receiver::newDataAvailable()
{
	unsigned int sizeToReadNext = 0;

	if( !m_buffer )
	{
		m_stateReadSize = true;
		m_bytesNeeded = sizeof(unsigned int);
		m_buffer.reset( new log4cplus::helpers::SocketBuffer(m_bytesNeeded) );
	}

	while( m_socket->bytesAvailable() )
	{
		// read outstanding data
		readDataToBuffer();

		if( m_bytesNeeded == 0 )
		{
			// interprete data
			if( m_stateReadSize )
			{
				sizeToReadNext = m_buffer->readInt();
				// Ignore message packets with 0 byte length. This is used for tunnel probing.
				if( sizeToReadNext != 0 )
					m_stateReadSize = false;
				else
					sizeToReadNext = sizeof(unsigned int);
			}
			else
			{
				emit newEntry( bufferToEntry() );

				m_stateReadSize = true;
				sizeToReadNext = sizeof(unsigned int);
			}

			// We will shutdown if we receive more then 1M of data.
			// This is a prevention from memory errors due to wrong data.
			if( sizeToReadNext > 1024*1024 )
			{
				shutdown();
				return;
			}

			m_buffer.reset( new log4cplus::helpers::SocketBuffer(sizeToReadNext) );
			m_bytesNeeded = sizeToReadNext;
		}
	}
}

TSharedLogEntry LogEntryParser_log4cplusSocket_Receiver::bufferToEntry()
{
	log4cplus::spi::InternalLoggingEvent event = readFromBuffer(*m_buffer);
#if QT_VERSION > 0x040700 //needs > Qt.4.7
	QDateTime timestamp( QDateTime::fromMSecsSinceEpoch ( qint64(event.getTimestamp().getTime()) * 1000 + ((qint64( event.getTimestamp().usec()/1000)%1000) ) ) );
#else
	// This is a workaround for older QT versions (<=4.7)
	QDateTime timestamp( QDateTime::fromTime_t(0) );
	timestamp = timestamp.addMSecs(qint64(event.getTimestamp().getTime()) * 1000 + ((qint64( event.getTimestamp().usec()/1000)%1000) ) );
#endif

	TSharedLogEntry entry = m_server->myFactory.generateLogEntry( timestamp );
	entry->setMessage( QString( event.getMessage().c_str() ) );

	boost::shared_ptr<QString> logLevel = m_server->m_loglevelStringOff;
	if( event.getLogLevel() >= log4cplus::OFF_LOG_LEVEL )
		logLevel = m_server->m_loglevelStringOff;
	else if( event.getLogLevel() >= log4cplus::FATAL_LOG_LEVEL )
		logLevel = m_server->m_loglevelStringFatal;
	else if( event.getLogLevel() >= log4cplus::ERROR_LOG_LEVEL )
		logLevel = m_server->m_loglevelStringError;
	else if( event.getLogLevel() >= log4cplus::WARN_LOG_LEVEL )
		logLevel = m_server->m_loglevelStringWarn;
	else if( event.getLogLevel() >= log4cplus::DEBUG_LOG_LEVEL )
		logLevel = m_server->m_loglevelStringDebug;
	else if( event.getLogLevel() >= log4cplus::TRACE_LOG_LEVEL )
		logLevel = m_server->m_loglevelStringTrace;

	entry->getAttributes().setAttribute( logLevel , 0 );
	entry->getAttributes().setAttribute( TSharedConstQString(new QString( event.getLoggerName().c_str() ) ), 1 );
	TSharedQString source = TSharedQString( new QString( event.getFile().c_str() ) );
	(*source) = (*source) + ":" + QString::number( event.getLine() );
	entry->getAttributes().setAttribute( source, 2 );
	entry->getAttributes().setAttribute( TSharedConstQString( new QString( event.getThread().c_str() ) ), 3 );
	entry->getAttributes().setAttribute( TSharedConstQString( new QString( event.getNDC().c_str() ) ), 4 );
	return entry;
}

void LogEntryParser_log4cplusSocket_Receiver::readDataToBuffer( )
{
	const qint64 toRead = std::min<qint64>( m_socket->bytesAvailable(), m_bytesNeeded );
	size_t position = m_buffer->getMaxSize() - m_bytesNeeded;

    qint64 res = m_socket->read( m_buffer->getBuffer() + position, toRead );
    m_bytesNeeded -= res;
}

void LogEntryParser_log4cplusSocket_Receiver::shutdown()
{
	if( m_socket->isValid() && m_socket->isOpen() )
	{
		m_socket->close();
	}
	deleteLater();
}

