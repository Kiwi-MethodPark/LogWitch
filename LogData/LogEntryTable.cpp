/*
 * LogEntryTable.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */
#include "LogEntryTable.h"

LogEntryTable::LogEntryTable(boost::shared_ptr<const LogEntryAttributeFactory> factory)
	: m_table()
	, m_attributeFactory( factory )
{

}

LogEntryTable::TLogEntryTable &LogEntryTable::getLogEntryTable()
{
	return m_table;
}

const LogEntryTable::TLogEntryTable &LogEntryTable::getLogEntryTable() const
{
	return m_table;
}

boost::shared_ptr<const LogEntryAttributeFactory> LogEntryTable::getLogEntryAttributeFactory() const
{
	return m_attributeFactory;
}
