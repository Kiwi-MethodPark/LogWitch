/*
 * LogEntryParser_log4cplusSocket.h
 *
 *  Created on: May 23, 2011
 *      Author: sven
 */

#ifndef LOGENTRYPARSER_LOG4CPLUSSOCKET_H_
#define LOGENTRYPARSER_LOG4CPLUSSOCKET_H_
#include <boost/scoped_ptr.hpp>
#include <log4cplus/helpers/socket.h>
#include <QtCore/QtCore>
#include <QtNetwork>

#include "LogData/LogEntryFactory.h"
#include "LogData/LogEntryParser.h"
#include <boost/shared_ptr.hpp>

class LogEntryParser_log4cplusSocket_ReceiverThread;

class LogEntryParser_log4cplusSocket
  : public QTcpServer
  , public LogEntryParser
{
	Q_OBJECT

	friend class LogEntryParser_log4cplusSocket_ReceiverThread;
public:
	LogEntryParser_log4cplusSocket( int port );

	virtual ~LogEntryParser_log4cplusSocket();

	void startEmiting();

	boost::shared_ptr<LogEntryParserModelConfiguration> getParserModelConfiguration() const;
public:
	void newEntryFromReceiver( TSharedLogEntry entry);

protected:
    void incomingConnection(int socketDescriptor);

signals:
	void newEntry( TSharedLogEntry );

private:
	int m_port;

	// std::list<LogEntryParser_log4cplusSocket_ReceiverThread *> m_myReceiverThreads;

	LogEntryFactory myFactory;

	boost::shared_ptr<LogEntryParserModelConfiguration> m_myModelConfig;

	boost::shared_ptr<QString> m_loglevelStringOff;
	boost::shared_ptr<QString> m_loglevelStringFatal;
	boost::shared_ptr<QString> m_loglevelStringError;
	boost::shared_ptr<QString> m_loglevelStringWarn;
	boost::shared_ptr<QString> m_loglevelStringDebug;
	boost::shared_ptr<QString> m_loglevelStringTrace;
};

class LogEntryParser_log4cplusSocket_ReceiverThread
	: public QThread
{
	Q_OBJECT
public:
	LogEntryParser_log4cplusSocket_ReceiverThread(int socketDescriptor, LogEntryParser_log4cplusSocket *parent);

	virtual ~LogEntryParser_log4cplusSocket_ReceiverThread();

	void shutdownSocket();

 signals:
	 void error(QTcpSocket::SocketError socketError);

protected:
	void run();

private:
	int m_clientsock;

	LogEntryParser_log4cplusSocket *m_parent;
};
#endif /* LOGENTRYPARSER_LOG4CPLUSSOCKET_H_ */
