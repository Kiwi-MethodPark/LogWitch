/*
 * LogEntryParser.h
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#ifndef LOGENTRYPARSER_H_
#define LOGENTRYPARSER_H_

/**
 * This class parses a line or more lines from a log file and passes the found
 * elements to the LogEntryFactory for generating a new LogEntry.
 */
class LogEntryParser {
public:
	virtual boost::shared_ptr<LogEntry> getNextLogEntry() = 0;

	virtual ~LogEntryParser() { }
};

#endif /* LOGENTRYPARSER_H_ */
