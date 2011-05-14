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
class LogEntryAttributeFactory;

class LogEntryTable
{
public:
	LogEntryTable(const LogEntryAttributeFactory *factory);

	typedef std::vector<boost::shared_ptr<LogEntry> > TLogEntryTable;

	TLogEntryTable &getLogEntryTable();
	const TLogEntryTable &getLogEntryTable() const;

	const LogEntryAttributeFactory *getLogEntryAttributeFactory() const;

private:
	TLogEntryTable m_table;

	const LogEntryAttributeFactory *m_attributeFactory;
};

#endif /* LOGENTRYTABLE_H_ */
