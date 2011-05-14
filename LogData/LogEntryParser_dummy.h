/*
 * LogEntryParser_dummy.h
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#ifndef LOGENTRYPARSER_DUMMY_H_
#define LOGENTRYPARSER_DUMMY_H_

#include "LogEntryParser.h"
#include "LogData/LogEntryFactory.h"

/**
 * This class is a dummy for testing without a valid logfile.
 * Generates dummy entries.
 */
class LogEntryParser_dummy: public LogEntryParser {
public:
	LogEntryParser_dummy();
	virtual ~LogEntryParser_dummy();

	virtual boost::shared_ptr<LogEntry> getNextLogEntry();

	virtual const LogEntryAttributeFactory *getLogEntryAttributeFactory() const;

private:
	 LogEntryFactory myFactory;

	int m_entries;
};

#endif /* LOGENTRYPARSER_DUMMY_H_ */
