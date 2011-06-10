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
            actions.back()->getData( var, role);
        }
    }

    //index.row();

    return var;
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
