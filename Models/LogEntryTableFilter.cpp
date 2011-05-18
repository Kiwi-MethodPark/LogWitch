/*
 * LogEntryTableFilter.cpp
 *
 *  Created on: May 18, 2011
 *      Author: sven
 */

#include "LogEntryTableFilter.h"
#include "Models/LogEntryTableModel.h"
#include "LogData/LogEntry.h"
#include "LogData/LogEntryAttributes.h"
#include "Assert.h"

LogEntryTableFilter::LogEntryTableFilter( QObject *parent)
	: QSortFilterProxyModel( parent )
	, m_model( NULL )
{
}

LogEntryTableFilter::~LogEntryTableFilter()
{
}

void LogEntryTableFilter::setSourceModel( QAbstractItemModel *model )
{
	QSortFilterProxyModel::setSourceModel( model );

	m_model = dynamic_cast<LogEntryTableModel *>( sourceModel() );
	LFA_ASSERT( m_model, "Invalid model given!" );
}

bool LogEntryTableFilter::filterAcceptsRow ( int sourceRow, const QModelIndex & sourceParent ) const
{
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    TconstSharedLogEntry entry = m_model->getEntryByIndex( index );

    if(*entry->getAttributes().getAttribute(0) == "ERROR" )
    {
    	return true;
    }
    else
    {
    	return false;
    }
}
