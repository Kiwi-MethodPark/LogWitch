/*
 * LogEntryFactory.h
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#ifndef LOGENTRYFACTORY_H_
#define LOGENTRYFACTORY_H_
#include <boost/shared_ptr.hpp>
#include "LogEntry.h"
#include "StringCache.h"

/**
 * This class generates a new log entry and ensures the uniqueness of
 * severity and source strings.
 */
class LogEntryFactory {
public:
	LogEntryFactory();

	boost::shared_ptr<LogEntry> generateLogEntry( const QDateTime &date, boost::shared_ptr<QString> source, boost::shared_ptr<QString> severity, const QString &message );

	StringCache &getSeverityCache( ) { return severityCache; }

	StringCache &getSourceCache() { return sourceCache; }

private:
	StringCache severityCache;

	StringCache sourceCache;
};

#endif /* LOGENTRYFACTORY_H_ */
