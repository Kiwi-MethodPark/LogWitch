/*
 * TableModelRules.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#include "TableModelRules.h"
#include "ActionRules/ActionDataRewriter.h"

TableModelRules::TableModelRules( QObject *parent )
    :QAbstractTableModel( parent )
{
    TSharedFilterRuleRaw row = TSharedFilterRuleRaw( new FilterRuleRaw );
    row->actionAsString = "Rewrite(FG:yellow)";
    row->expressionAsString = "Severity == \"DEBUG\"";
    m_table.push_back( row );

    row = TSharedFilterRuleRaw( new FilterRuleRaw );
    row->actionAsString = "Rewrite(FG:green)";
    row->expressionAsString = "Severity == \"TRACE\"";
    m_table.push_back( row );

    row = TSharedFilterRuleRaw( new FilterRuleRaw );
    row->actionAsString = "Rewrite(FG:red)";
    row->expressionAsString = "Severity == \"FATAL\"";
    m_table.push_back( row );

//    TSharedTableRow row( new tableRow );
//    row->expression = "Severity == \"DEBUG\"";
//    TSharedActionDataRewriter action( new ActionDataRewriter );
//    action->addChangeSet( Qt::yellow, Qt::ForegroundRole );
//    row->action = action;
//    m_table.push_back( row );
//    m_executorTable.push_back( TSharedTableExecutor(new tableExecutor) );
}

TableModelRules::~TableModelRules()
{
}

int TableModelRules::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_table.size();
}

int TableModelRules::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_columnCount;
}

QVariant TableModelRules::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
     return QVariant();

    if (index.column() >= m_columnCount
         || index.column() < 0
         || index.row() < 0
         || index.row() >= (m_table.size() ) )
     return QVariant();

    TSharedConstFilterRuleRaw row = m_table[index.row()];

    if( index.column() == 0 )
    {
        if (role == Qt::DisplayRole || role == Qt::EditRole)
        {
            return row->expressionAsString;
        }
    }
    else if( index.column() == 1 )
    {
        if( role == Qt::EditRole )
        {
            return row->actionAsString;
        }

        if( row->actionDisplayer )
        {
            return row->actionDisplayer->toDisplay( role );
        }
        else if (role == Qt::DisplayRole )
        {
            return row->actionAsString;
        }
    }

    return QVariant();
}

Qt::ItemFlags TableModelRules::flags(const QModelIndex &  ) const
{
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant TableModelRules::headerData(int section, Qt::Orientation orientation, int role) const
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

bool TableModelRules::setData( const QModelIndex &index, const QVariant& value , int role  )
{
    if( !index.isValid())
        return false;

    if (index.column() >= m_columnCount
         || index.column() < 0
         || index.row() < 0
         || index.row() >= (m_table.size() ) )
        return false;

    if (role != Qt::EditRole)
        return false;

    TSharedFilterRuleRaw row = m_table[index.row()];

    if( index.column() == 0 )
    {
        row->expressionAsString = value.toString();
        return true;
    }
    else if( index.column() == 1 )
    {
        row->actionAsString = value.toString();
        return true;
    }

    return false;
}
