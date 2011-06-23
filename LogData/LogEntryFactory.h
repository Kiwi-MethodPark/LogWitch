/*
 * LogEntryFactory.h
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#ifndef LOGENTRYFACTORY_H_
#define LOGENTRYFACTORY_H_
#include <boost/shared_ptr.hpp>
#include "LogData/LogEntry.h"

class LogEntryAttributeFactory;
class QDateTime;
class QString;

/**
 * This class generates a new log entry and ensures the uniqueness of
 * severity and source strings.
 */
class LogEntryFactory {
public:
	LogEntryFactory();

	TSharedLogEntry generateLogEntry( );

	boost::shared_ptr<const LogEntryAttributeFactory> getLogEntryAttributeFactory() const;

	boost::shared_ptr<LogEntryAttributeFactory> &getLogEntryAttributeFactory();

private:
	boost::shared_ptr<LogEntryAttributeFactory> m_indexedAttributesFactory;
};

#endif /* LOGENTRYFACTORY_H_ */
