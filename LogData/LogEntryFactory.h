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
#include "LogData/LogEntryAttributeFactory.h"

/**
 * This class generates a new log entry and ensures the uniqueness of
 * severity and source strings.
 */
class LogEntryFactory {
public:
	LogEntryFactory();

	boost::shared_ptr<LogEntry> generateLogEntry( const QDateTime &date, const QString &message );

private:
	LogEntryAttributeFactory m_indexedAttributesFactory;
};

#endif /* LOGENTRYFACTORY_H_ */
