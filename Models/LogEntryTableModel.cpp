/*
 * LogEntryTableModel.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogEntryTableModel.h"

#include <QtCore/QtCore>
#include <QtCore/QVariant>
#include <QDateTime>

#include "LogData/LogEntry.h"
#include "LogData/LogEntryTable.h"
#include "LogData/LogEntryAttributes.h"
#include "LogData/LogEntryAttributeFactory.h"

LogEntryTableModel::LogEntryTableModel(boost::shared_ptr<const LogEntryTable> table )
	: m_table( table )
{

}

LogEntryTableModel::~LogEntryTableModel()
{

}


int LogEntryTableModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return m_table->getLogEntryTable().size();
}

int LogEntryTableModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return m_table->getLogEntryAttributeFactory()->getNumberOfFields( ) + 2;
}

QVariant LogEntryTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= (m_table->getLogEntryTable().size() + 2) || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
    	LogEntryTable::TLogEntryTable &getLogEntryTable();

    	boost::shared_ptr<LogEntry> entry = m_table->getLogEntryTable()[index.row()];

    	if( index.column() == 0 )
    		return entry->getTimestamp();
    	if( index.column() == 1 )
    		return entry->getMessage();

    	return *(entry->getAttributes().getAttribute(index.column()-2 ));
    }
    return QVariant();
}

QVariant LogEntryTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (section >= (m_table->getLogEntryTable().size() + 2) || section < 0)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
    	if( section == 0 )
    		return tr("Timestamp");
    	if( section == 1 )
    		return tr("Message text");

    	return m_table->getLogEntryAttributeFactory()->getDescription( section-2 );
    }
    return QVariant();
}

Qt::ItemFlags LogEntryTableModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index);
}
