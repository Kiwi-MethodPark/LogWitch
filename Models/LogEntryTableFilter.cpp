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
#include "ActionRules/ActionDiscardRow.h"

LogEntryTableFilter::LogEntryTableFilter( QObject *parent)
	: QSortFilterProxyModel( parent )
	, m_model( NULL )
    , m_ruleTable( new RuleTable )
{
    QObject::connect(&m_filterChain, SIGNAL(filterUpdateFinished()),
                     this, SLOT(invalidate()));
    QObject::connect( m_ruleTable.get(), SIGNAL(changed()),
            this, SLOT(invalidate()));
}

LogEntryTableFilter::~LogEntryTableFilter()
{
}

void LogEntryTableFilter::addFilter( boost::shared_ptr<LogEntryFilter> flt )
{
	m_filterChain.addFilter( flt );
}

TSharedRuleTable LogEntryTableFilter:: getRuleTable()
{
    return m_ruleTable;
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
            QModelIndex srcIdx = mapToSource( index );

            std::list<  TconstSharedActionDataRewriter >::iterator it;
            for( it = actions.begin(); it != actions.end(); ++it  )
                (*it)->modifyData( var, srcIdx.column(), role);
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
    bool filterAccept = true;

    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    TconstSharedLogEntry entry = m_model->getEntryByIndex( index );

    filterAccept &= m_filterChain.filterEntry( entry );

    if( m_ruleTable )
    {
        if( m_ruleTable->isActionMatched<TconstSharedActionDiscardRow>( entry ) )
        {
            filterAccept = false;
        }
    }

    return filterAccept;
}
