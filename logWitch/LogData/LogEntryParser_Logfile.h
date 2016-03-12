/*
 * LogEntryParser_Logfile.h
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#ifndef LOGENTRYPARSER_LOGFILE_H_
#define LOGENTRYPARSER_LOGFILE_H_

#include "LogEntryParser.h"

#include <boost/scoped_ptr.hpp>

#include <QFile>
#include <QTextStream>
#include <QtCore/QtCore>
#include <QRegExp>
#include "LogEntryFactory.h"

class QRegExp;
class LogEntryOld;
class LogEntryParserModelConfiguration;
class ParserStreamGetter;

class LogEntryParser_Logfile
	: public QThread
	, public LogEntryParser
{
	Q_OBJECT
public:
	LogEntryParser_Logfile( boost::shared_ptr<ParserStreamGetter> getter );

	~LogEntryParser_Logfile();

	void startEmiting();

	bool initParser();

	void run();

	virtual boost::shared_ptr<LogEntryParserModelConfiguration> getParserModelConfiguration() const;

	QString getName() const;
signals:
	void newEntry( TconstSharedNewLogEntryMessage );

    void signalError( QString error );

  void finished();

private:
	virtual TSharedLogEntry getNextLogEntry();

	bool m_abort;

	boost::shared_ptr<ParserStreamGetter> m_getter;

	boost::shared_ptr<QTextStream> m_logfileStream;

	TSharedLogEntry m_entry;

	QString stashedLine;

	QString message;

	boost::scoped_ptr<QRegExp> lineMessageRegex;

	QRegExp cellRegex;

	QString timeFormat;

	boost::shared_ptr<LogEntryFactory> myFactory;

	boost::shared_ptr<LogEntryParserModelConfiguration> m_myModelConfig;

	int m_logEntryNumber;


};

#endif /* LOGENTRYPARSER_LOGFILE_H_ */
