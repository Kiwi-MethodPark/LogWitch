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


namespace
{
	qint64 read( log4cplus::helpers::SocketBuffer &buffer, QTcpSocket &socket )
	{
	    qint64 read = 0;

	    do
	    {
	    	socket.waitForReadyRead(100);
	    	if( !socket.isValid() )
	    		return 0;

	        qint64 res = socket.read( buffer.getBuffer() + read,
	            buffer.getMaxSize() - read);
	        if( res < 0 )
	        {
	        	qDebug() << "SOCKET error: res " << res <<" read" << read << " of " << buffer.getMaxSize();
	            return 0;
	        }
	        read += res;
	    } while( read < static_cast<qint64>(buffer.getMaxSize()) );

		qDebug() << "read" << read << " of " << buffer.getMaxSize();

	    return read;
	}
}
LogEntryParser_log4cplusSocket_ReceiverThread::LogEntryParser_log4cplusSocket_ReceiverThread( int socketDescriptor, LogEntryParser_log4cplusSocket *parent)
: m_clientsock(socketDescriptor)
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
	// m_clientsock.close();
}

void LogEntryParser_log4cplusSocket_ReceiverThread::run()
{
	QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(m_clientsock)) {
        emit error(tcpSocket.error());
        qDebug() << "Socket error: " << tcpSocket.error();
        return;
    }

	forever
	{
		log4cplus::helpers::SocketBuffer msgSizeBuffer(sizeof(unsigned int));
		if(!read(msgSizeBuffer,tcpSocket))
		{
			qDebug() << "Wrong message size (1)";
			qDebug() << "Socket error: " << tcpSocket.error();
			return;
		}

		unsigned int msgSize = msgSizeBuffer.readInt();

		log4cplus::helpers::SocketBuffer buffer(msgSize);
		if(!read(buffer,tcpSocket))
		{
			qDebug() << "Wrong message size (2)";
			qDebug() << "Socket error: " << tcpSocket.error();
			return;
		}

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
		TSharedQString source = TSharedQString( new QString( event.getFile().c_str() ) );
		(*source) = (*source) + ":" + QString::number( event.getLine() );
		entry->getAttributes().setAttribute( source, 2 );
		entry->getAttributes().setAttribute( TSharedConstQString( new QString( event.getThread().c_str() ) ), 3 );
		entry->getAttributes().setAttribute( TSharedConstQString( new QString( event.getNDC().c_str() ) ), 4 );

		m_parent->newEntry( entry );
	}

   tcpSocket.disconnectFromHost();
   tcpSocket.waitForDisconnected();
}


LogEntryParser_log4cplusSocket::LogEntryParser_log4cplusSocket( int port )
	:m_port(port)
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
	m_myModelConfig->setEntryToTextFormater( boost::shared_ptr<EntryToTextFormater>( new EntryToTextFormater_Logfile ) );

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
}

LogEntryParser_log4cplusSocket::~LogEntryParser_log4cplusSocket()
{
	qDebug() << "called: ~LogEntryParser_log4cplusSocket";
	// First stop socket waiter
//	m_abort = true;
//	m_serverSocket->close();
//	qDebug() << "wait for server thread ... ";
//	wait();
//
//	qDebug() << "shutting down receiver threads ... ";
//
//	// then stop receiver threads
//	// m_myReceiverThreads
//	while( !m_myReceiverThreads.empty() )
//	{
//		m_myReceiverThreads.front()->shutdownSocket();
//		m_myReceiverThreads.front()->wait();
//		delete m_myReceiverThreads.front();
//		m_myReceiverThreads.pop_front();
//	}
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
	listen ( QHostAddress::Any, m_port );
}

void LogEntryParser_log4cplusSocket::incomingConnection(int socketDescriptor)
{
	qDebug() << "Incomming connection ... dispatching";

	LogEntryParser_log4cplusSocket_ReceiverThread *thread = new LogEntryParser_log4cplusSocket_ReceiverThread(socketDescriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}

