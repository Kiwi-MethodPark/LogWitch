/*
 * TableModelRulesCompiledCompiled.cpp
 *
 *  Created on: Jun 10, 2011
 *      Author: sven
 */

#include "TableModelRulesCompiled.h"

#include <boost/bind.hpp>

#include "ActionRules/ActionDataRewriter.h"
#include "ActionRules/FilterRuleCompiled.h"

TableModelRulesCompiled::TableModelRulesCompiled( QObject *parent, TSharedConstLogEntryParserModelConfiguration cfg )
    :QAbstractTableModel( parent )
    , m_configuration( cfg )
{
}

TableModelRulesCompiled::~TableModelRulesCompiled()
{
}

int TableModelRulesCompiled::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_table.size();
}

int TableModelRulesCompiled::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_columnCount;
}

QVariant TableModelRulesCompiled::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
     return QVariant();

    if (index.column() >= m_columnCount
         || index.column() < 0
         || index.row() < 0
         || index.row() >= (m_table.size() ) )
     return QVariant();

    TSharedFilterRuleCompiled row = m_table[index.row()];

    if( !row->validWithinContext() )
    {
        if( !row->getDescription()->isExpressionOk() )
        {
            if( role == Qt::ToolTipRole )
            {
                return row->getDescription()->getExpressionError();
            }
            if( role == Qt::BackgroundColorRole )
                return Qt::red;
        }
        else
        {
            if( role == Qt::ToolTipRole )
            {
                return QString("The expression is not valid within context. One or more fields are not defined within model.");
            }
            if( role == Qt::BackgroundColorRole )
                return Qt::gray;
        }
    }

    if( index.column() == 0 )
    {
        if (role == Qt::DisplayRole)
        {
            return row->getDescription()->expressionAsString();
        }
    }
    else if( index.column() == 1 )
    {
        if( role == Qt::EditRole )
        {
            return row->getDescription()->actionString();
        }

        if( !row->getDescription()->isActionOk())
        {
            if( role == Qt::BackgroundColorRole )
            {
                return Qt::red;
            }
            if( role == Qt::ToolTipRole )
            {
                return row->getDescription()->getActionError();
            }
        }

        if( row->getDescription()->getActionDisplayer() )
        {
            return row->getDescription()->getActionDisplayer()->toDisplay( role );
        }
        else if (role == Qt::DisplayRole )
        {
            return row->getDescription()->actionString();
        }
    }

    return QVariant();
}

Qt::ItemFlags TableModelRulesCompiled::flags(const QModelIndex & index ) const
{
    return QAbstractItemModel::flags( index );
}

QVariant TableModelRulesCompiled::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
    {
        if (orientation == Qt::Horizontal)
        {
            if( section == 0 )
                return QString(tr("Expression"));
            if( section == 1 )
                return QString(tr("Action"));
        }
    }

    return QVariant();
}

bool TableModelRulesCompiled::setData( const QModelIndex &index, const QVariant& , int  )
{
    if( !index.isValid())
        return false;

    return false;
}

TSharedConstFilterRuleRaw compiledToRaw (TSharedFilterRuleCompiled c)
{
  return c->getDescription();
}

void TableModelRulesCompiled::appendRule( TSharedFilterRuleRaw rule )
{
    // Check if we have already this entry
    TRuleTable::iterator it;
    it = std::find_if(m_table.begin(), m_table.end(), boost::bind( &compiledToRaw, _1 ) == rule );
    if( it != m_table.end() )
        return;

    TSharedFilterRuleCompiled cr( new FilterRuleCompiled( rule, m_configuration ) );

    int newPos = m_table.size();
    beginInsertRows( QModelIndex(), newPos, newPos );
    m_table.push_back( cr );
    endInsertRows();
}
