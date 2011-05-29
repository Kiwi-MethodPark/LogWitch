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
class LogEntry;
class LogEntryParserModelConfiguration;

class LogEntryParser_Logfile
	: public QThread
	, public LogEntryParser
{
	Q_OBJECT
public:
	LogEntryParser_Logfile( const QString &filename );

	~LogEntryParser_Logfile();

	void startEmiting();

	void run();

	virtual boost::shared_ptr<LogEntryParserModelConfiguration> getParserModelConfiguration() const;

	QString getName() const{ return m_Name; }
signals:
	void newEntry( TSharedLogEntry );

    void signalError( QString error );

private:
	void init();

	virtual TSharedLogEntry getNextLogEntry();

	bool m_abort;

	QFile logfile;

	QTextStream logfileStream;

	bool logfileStreamReady;

	TSharedLogEntry entry;

	QString stashedLine;

	QString message;

	boost::scoped_ptr<QRegExp> lineMessageRegex;

	QRegExp cellRegex;

	QString timeFormat;

	LogEntryFactory myFactory;

	boost::shared_ptr<LogEntryParserModelConfiguration> m_myModelConfig;

	QString m_Name;


};

#endif /* LOGENTRYPARSER_LOGFILE_H_ */
