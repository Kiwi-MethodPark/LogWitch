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
#include "LogEntryFactory.h"

class QRegExp;
class LogEntry;

class LogEntryParser_Logfile : public LogEntryParser
{
public:
	LogEntryParser_Logfile( const QString &filename );

	virtual TSharedLogEntry getNextLogEntry();

	virtual boost::shared_ptr<const LogEntryAttributeFactory> getLogEntryAttributeFactory() const;

private:
	void init();

	QFile logfile;

	QTextStream logfileStream;

	bool logfileStreamReady;

	QString stashedLine;

	boost::scoped_ptr<QRegExp> lineMessageRegex;

	boost::shared_ptr<const LogEntryFactory> m_LogEntryFactory;


};

#endif /* LOGENTRYPARSER_LOGFILE_H_ */
