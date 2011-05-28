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
#include "LogData/LogEntryParserModelConfiguration.h"

LogEntryTableModel::LogEntryTableModel( boost::shared_ptr<LogEntryParser> parser )
	: m_table( )
	, m_modelConfiguration( parser->getParserModelConfiguration() )
	, m_dateTimeConversionString("dd.MM.yyyy hh:mm:ss.zzz")
	, m_entryLoader( parser )
	, m_ModelName("Untitled")
{
    QObject::connect(dynamic_cast<QObject*>(parser.get()), SIGNAL(newEntry( TSharedLogEntry)),
                     this, SLOT(insertEntry( TSharedLogEntry )) );

    m_ModelName = parser->getName();
}

LogEntryTableModel::~LogEntryTableModel()
{

}

void LogEntryTableModel::startModel()
{
	m_entryLoader->startEmiting();
}

boost::shared_ptr<const LogEntryParserModelConfiguration> LogEntryTableModel::getParserModelConfiguration() const
{
	return m_modelConfiguration;
}

int LogEntryTableModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return m_table.size();
}

int LogEntryTableModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	int value = m_modelConfiguration->getLogEntryAttributeFactory()->getNumberOfFields( ) + m_fixedFirstColumns;
	return value;
}

TconstSharedLogEntry LogEntryTableModel::getEntryByIndex( const QModelIndex &index ) const
{
    if (index.column() >= (m_modelConfiguration->getLogEntryAttributeFactory()->getNumberOfFields( ) + m_fixedFirstColumns )
    		|| index.column() < 0
    		|| index.row() < 0
    		|| index.row() >= (m_table.size() ) )
        return TconstSharedLogEntry();

	return m_table[index.row()];
}

QVariant LogEntryTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.column() >= (m_modelConfiguration->getLogEntryAttributeFactory()->getNumberOfFields( ) + m_fixedFirstColumns )
    		|| index.column() < 0
    		|| index.row() < 0
    		|| index.row() >= (m_table.size() ) )
        return QVariant();

    if (role == Qt::DisplayRole)
    {
    	TconstSharedLogEntry entry = m_table[index.row()];

    	if( index.column() == 0 )
    		return index.row() + 1;
    	if( index.column() == 2 )
    		return entry->getTimestamp().toString( m_dateTimeConversionString  );
    	if( index.column() == 1 )
    		return entry->getMessage();


    	return *(entry->getAttributes().getAttribute(index.column()-m_fixedFirstColumns ));
    }
    return QVariant();
}

QVariant LogEntryTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section >= (m_modelConfiguration->getLogEntryAttributeFactory()->getNumberOfFields( ) + m_fixedFirstColumns) || section < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
		if (orientation == Qt::Horizontal)
		{
			if( section == 0 )
				return tr("Nr.");
			if( section == 2 )
				return tr("Timestamp");
			if( section == 1 )
				return tr("Message text");

			return m_modelConfiguration->getLogEntryAttributeFactory()->getDescription( section-m_fixedFirstColumns );
		}
    }
    else if( role == 512 )
    {
    	int rVal = 150;

    	if( section == 0 )
    		rVal = 60;
    	else if( section == 1 )
    		rVal = 500;
    	else if( section == 2 )
    		rVal = 180;
    	else
    		rVal = m_modelConfiguration->getFieldWidthHint( section - m_fixedFirstColumns );

    	return rVal;
    }

    return QVariant();
}

void LogEntryTableModel::insertEntry( TSharedLogEntry entry )
{
	int newPos = m_table.size();
	beginInsertRows( QModelIndex(), newPos, newPos );
	m_table.push_back( entry );
	endInsertRows();
}
