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
    QObject::connect(dynamic_cast<QObject*>(parser.get()), SIGNAL(signalError( QString )),
                     this, SLOT(signalErrorFromParser( QString )) );

    m_ModelName = parser->getName();
}

LogEntryTableModel::~LogEntryTableModel()
{

}

void LogEntryTableModel::startModel()
{
	m_entryLoader->startEmiting();
}

void LogEntryTableModel::signalErrorFromParser( QString error )
{
    qDebug() << "error from parser received: " << error;
    emit signalError( error );
}

TSharedConstLogEntryParserModelConfiguration LogEntryTableModel::getParserModelConfiguration() const
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
	int value = m_modelConfiguration->getLogEntryAttributeFactory()->getNumberOfFields( );
	return value;
}

TconstSharedLogEntry LogEntryTableModel::getEntryByIndex( const QModelIndex &index ) const
{
    if (index.column() >= (m_modelConfiguration->getLogEntryAttributeFactory()->getNumberOfFields( ) )
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

    if (index.column() >= (m_modelConfiguration->getLogEntryAttributeFactory()->getNumberOfFields( ) )
    		|| index.column() < 0
    		|| index.row() < 0
    		|| index.row() >= (m_table.size() ) )
        return QVariant();

    if (role == Qt::DisplayRole)
    {
    	TconstSharedLogEntry entry = m_table[index.row()];
    	return *(entry->getAttributes().getAttribute(index.column() ));
    }
    return QVariant();
}

QVariant LogEntryTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section >= (m_modelConfiguration->getLogEntryAttributeFactory()->getNumberOfFields( ) ) || section < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
		if (orientation == Qt::Horizontal)
		{
			return m_modelConfiguration->getLogEntryAttributeFactory()->getDescription( section );
		}
    }
    else if( role == 512 )
    {
    	int rVal = 150;

  		rVal = m_modelConfiguration->getFieldWidthHint( section );

    	return rVal;
    }

    return QVariant();
}

void LogEntryTableModel::clearTable()
{
    beginResetModel();
    m_table.clear();
    endResetModel();
}

void LogEntryTableModel::insertEntry( TSharedLogEntry entry )
{
	int newPos = m_table.size();
	beginInsertRows( QModelIndex(), newPos, newPos );
	m_table.push_back( entry );
	endInsertRows();
}
