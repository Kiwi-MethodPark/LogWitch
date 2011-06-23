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
#include "ActionRules/ActionDataRewriter.h"

LogEntryTableFilter::LogEntryTableFilter( QObject *parent)
	: QSortFilterProxyModel( parent )
	, m_model( NULL )
{
    QObject::connect(&m_filterChain, SIGNAL(filterUpdateFinished()),
                     this, SLOT(invalidate()));
}

LogEntryTableFilter::~LogEntryTableFilter()
{
}

void LogEntryTableFilter::addFilter( boost::shared_ptr<LogEntryFilter> flt )
{
	m_filterChain.addFilter( flt );
}

void LogEntryTableFilter::setRuleTable( TconstSharedRuleTable table )
{
    m_ruleTable = table;
    QObject::connect( m_ruleTable.get(), SIGNAL(changed()),
            this, SLOT(invalidate()));
}

QVariant LogEntryTableFilter::data(const QModelIndex &index, int role) const
{
    QVariant var = QSortFilterProxyModel::data( index, role );

    if( m_ruleTable )
    {
        TconstSharedLogEntry entry = m_model->getEntryByIndex( mapToSource( index ) );
        std::list<  TconstSharedActionDataRewriter > actions;
        if( m_ruleTable->getMatchedActionsForType( actions, entry ) )
        {
            std::list<  TconstSharedActionDataRewriter >::iterator it;
            for( it = actions.begin(); it != actions.end(); ++it  )
                (*it)->modifyData( var, index.column(), role);
        }
    }

    return var;
}

LogEntryTableModel *LogEntryTableFilter::getSourceModel() const
{
    return m_model;
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

    return m_filterChain.filterEntry( entry );
}
