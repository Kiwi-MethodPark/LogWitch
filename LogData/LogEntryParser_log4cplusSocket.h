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

#include "LogData/LogEntryFactory.h"
#include "LogData/LogEntryParser.h"

class LogEntryParser_log4cplusSocket_ReceiverThread;

class LogEntryParser_log4cplusSocket
  : public QThread
  , public LogEntryParser
{
	Q_OBJECT

	friend class LogEntryParser_log4cplusSocket_ReceiverThread;
public:
	LogEntryParser_log4cplusSocket( int port );

	virtual ~LogEntryParser_log4cplusSocket();

	void run();

	void startEmiting();

	boost::shared_ptr<LogEntryParserModelConfiguration> getParserModelConfiguration() const;
public:
	void newEntryFromReceiver( TSharedLogEntry entry);

signals:
	void newEntry( TSharedLogEntry );

private:
	int m_port;

	bool m_abort;

	std::list<LogEntryParser_log4cplusSocket_ReceiverThread *> m_myReceiverThreads;

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
	LogEntryParser_log4cplusSocket_ReceiverThread(log4cplus::helpers::Socket sock, LogEntryParser_log4cplusSocket *parent);

	virtual ~LogEntryParser_log4cplusSocket_ReceiverThread();

	void shutdownSocket();

protected:
	void run();

private:
	log4cplus::helpers::Socket m_clientsock;

	LogEntryParser_log4cplusSocket *m_parent;
};
#endif /* LOGENTRYPARSER_LOG4CPLUSSOCKET_H_ */
