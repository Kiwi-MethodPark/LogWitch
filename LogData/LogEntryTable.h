/*
 * LogEntryTable.h
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#ifndef LOGENTRYTABLE_H_
#define LOGENTRYTABLE_H_
#include <vector>
#include <boost/shared_ptr.hpp>

class LogEntry;

typedef std::vector<boost::shared_ptr<LogEntry> > TLogEntryTable;

#endif /* LOGENTRYTABLE_H_ */
