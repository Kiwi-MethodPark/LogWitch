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
#include "LogData/LogEntry.h"

class LogEntryAttributeFactory;

class LogEntryTable
{
public:
	LogEntryTable( boost::shared_ptr<const LogEntryAttributeFactory> factory);

	typedef std::vector<TSharedLogEntry > TLogEntryTable;

	TLogEntryTable &getLogEntryTable();
	const TLogEntryTable &getLogEntryTable() const;

	boost::shared_ptr<const LogEntryAttributeFactory> getLogEntryAttributeFactory() const;

private:
	TLogEntryTable m_table;

	boost::shared_ptr<const LogEntryAttributeFactory> m_attributeFactory;
};

#endif /* LOGENTRYTABLE_H_ */
