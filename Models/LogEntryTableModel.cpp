/*
 * LogEntryTableModel.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogEntryTableModel.h"

#include <iostream>

#include <QtCore/QtCore>
#include <QtCore/QVariant>
#include <QDateTime>

#include "LogData/LogEntry.h"
#include "LogData/LogEntryAttributes.h"
#include "LogData/LogEntryAttributeFactory.h"
#include "LogData/LogEntryParser.h"
#include "LogData/LogEntryTable.h"

LogEntryTableModel::LogEntryTableModel( boost::shared_ptr<LogEntryParser> parser )
	: m_table( new LogEntryTable( parser->getLogEntryAttributeFactory() ) )
	, m_dateTimeConversionString("dd.MM.yyyy hh:mm:ss.zzz")
	, m_entryLoader( parser )
{
    QObject::connect(dynamic_cast<QObject*>(parser.get()), SIGNAL(newEntry( TSharedLogEntry)),
                     this, SLOT(insertEntry( TSharedLogEntry )) );

	m_entryLoader->startEmiting();
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
	int value = m_table->getLogEntryAttributeFactory()->getNumberOfFields( ) + 2;
	return value;
}

TconstSharedLogEntry LogEntryTableModel::getEntryByIndex( const QModelIndex &index ) const
{
    if (index.column() >= (m_table->getLogEntryAttributeFactory()->getNumberOfFields( ) + 2 )
    		|| index.column() < 0
    		|| index.row() < 0
    		|| index.row() >= (m_table->getLogEntryTable().size() ) )
        return TconstSharedLogEntry();

	return m_table->getLogEntryTable()[index.row()];
}

QVariant LogEntryTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.column() >= (m_table->getLogEntryAttributeFactory()->getNumberOfFields( ) + 2 )
    		|| index.column() < 0
    		|| index.row() < 0
    		|| index.row() >= (m_table->getLogEntryTable().size() ) )
        return QVariant();

    if (role == Qt::DisplayRole)
    {
    	LogEntryTable::TLogEntryTable &getLogEntryTable();

    	TSharedLogEntry entry = m_table->getLogEntryTable()[index.row()];

    	if( index.column() == 0 )
    		return entry->getTimestamp().toString( m_dateTimeConversionString  );
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

    if (section >= (m_table->getLogEntryAttributeFactory()->getNumberOfFields( ) + 2) || section < 0)
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

void LogEntryTableModel::insertEntry( TSharedLogEntry entry )
{
	int newPos = m_table->getLogEntryTable().size();
	beginInsertRows( QModelIndex(), newPos, newPos );
	m_table->getLogEntryTable().push_back( entry );
	endInsertRows();
}
