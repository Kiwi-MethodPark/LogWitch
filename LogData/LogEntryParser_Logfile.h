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

class QRegExp;

class LogEntryParser_Logfile : public LogEntryParser
{
public:
	LogEntryParser_Logfile( const QString &filename );

	virtual boost::shared_ptr<LogEntry> getNextLogEntry();

private:
	QFile logfile;

	QTextStream logfileStream;

	bool logfileStreamReady;

	QString stashedLine;

	boost::scoped_ptr<QRegExp> lineMessageRegex;


};

#endif /* LOGENTRYPARSER_LOGFILE_H_ */
