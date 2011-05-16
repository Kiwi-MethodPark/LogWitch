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

#include "LogEntryFactory.h"
#include <QRegExp>

class QRegExp;
class LogEntry;

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

	virtual boost::shared_ptr<const LogEntryAttributeFactory> getLogEntryAttributeFactory() const;

signals:
	void newEntry( TSharedLogEntry );

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


};

#endif /* LOGENTRYPARSER_LOGFILE_H_ */
