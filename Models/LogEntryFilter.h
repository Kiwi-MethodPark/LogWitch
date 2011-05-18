/*
 * LogEntryFilter.h
 *
 *  Created on: May 18, 2011
 *      Author: sven
 */

#ifndef LOGENTRYFILTER_H_
#define LOGENTRYFILTER_H_
#include "LogData/LogEntry.h"

class LogEntryFilter {
public:
	virtual ~LogEntryFilter();

	virtual bool filterEntry( TconstSharedLogEntry entry ) const = 0;
};

#endif /* LOGENTRYFILTER_H_ */
