/*
 * LogEntryParser.h
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#ifndef LOGENTRYPARSER_H_
#define LOGENTRYPARSER_H_
#include <boost/shared_ptr.hpp>

 #include <QtGui>
#include <QtCore/QObject>

#include "LogData/LogEntry.h"

class LogEntryAttributeFactory;

/**
 * This class parses a line or more lines from a log file and passes the found
 * elements to the LogEntryFactory for generating a new LogEntry.
 */
class LogEntryParser
{
public:
	virtual ~LogEntryParser(){}

	/**
	 * This starts the parser after you have connected to the signals.
	 */
	virtual void startEmiting() = 0;

	virtual boost::shared_ptr<const LogEntryAttributeFactory> getLogEntryAttributeFactory() const = 0;

	// This block is the later called signals methods.
protected:
	virtual void newEntry( TSharedLogEntry ) = 0;
};

#endif /* LOGENTRYPARSER_H_ */
