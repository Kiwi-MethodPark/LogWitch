/*
 * TableModelRulesCompiledCompiled.cpp
 *
 *  Created on: Jun 10, 2011
 *      Author: sven
 */

#include "TableModelRulesCompiled.h"

#include "ActionRules/ActionDataRewriter.h"
#include "ActionRules/FilterRuleCompiled.h"

TableModelRulesCompiled::TableModelRulesCompiled( QObject *parent )
    :QAbstractTableModel( parent )
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

    if( index.column() == 0 )
    {
        if (role == Qt::DisplayRole)
        {
            return row->parentRule->expressionAsString;
        }
    }
    else if( index.column() == 1 )
    {
        if( row->parentRule->actionDisplayer )
        {
            return row->parentRule->actionDisplayer->toDisplay( role );
        }
        else if (role == Qt::DisplayRole )
        {
            return row->parentRule->actionAsString;
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
