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
    row->actionString( "Rewrite(FG:yellow)" );
    row->expressionAsString("Severity == \"DEBUG\"");
    m_table.push_back( row );

    row = TSharedFilterRuleRaw( new FilterRuleRaw );
    row->actionString("Rewrite(FG:green)");
    row->expressionAsString("Severity == \"TRACE\"");
    m_table.push_back( row );

    row = TSharedFilterRuleRaw( new FilterRuleRaw );
    row->actionString("Rewrite(FG:red)");
    row->expressionAsString("Severity == \"FATAL\"");
    m_table.push_back( row );
}

TableModelRules::~TableModelRules()
{
}

void TableModelRules::insertEmptyRule()
{
    TSharedFilterRuleRaw row = TSharedFilterRuleRaw( new FilterRuleRaw );
    row->actionString( "" );
    row->expressionAsString("");

    int newPos = m_table.size();
    beginInsertRows( QModelIndex(), newPos, newPos );
    m_table.push_back( row );
    endInsertRows();
}

namespace
{
    // Sorting in backward order.
    bool indexRow(const QModelIndex &s1, const QModelIndex &s2)
    {
        return s1.row() > s2.row();
    }
}

void TableModelRules::removeRules( const QModelIndexList &idxList )
{
    // Removing many is complex, because the rows within idxList always reference
    // to the state before removing a row. So we sort everything in reverse order
    // and then start removing from the back on.
    QModelIndexList myList( idxList );
    qSort( myList.begin(), myList.end(), indexRow );

    QModelIndexList::Iterator it;
    for( it = myList.begin(); it != myList.end(); ++it )
    {
        beginRemoveRows(QModelIndex(), it->row(), it->row());
        m_table.erase( m_table.begin() + it->row() );
        endRemoveRows();
    }
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

TSharedFilterRuleRaw TableModelRules::getRaw(const QModelIndex &index) const
{
    if (!index.isValid())
        return TSharedFilterRuleRaw();
    if (index.column() >= m_columnCount
         || index.column() < 0
         || index.row() < 0
         || index.row() >= (m_table.size() ) )
        return TSharedFilterRuleRaw();

    return m_table[index.row()];
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
            return row->expressionAsString();
        }

        if( !row->isExpressionOk())
        {
            if( role == Qt::BackgroundColorRole )
            {
                return Qt::red;
            }
            if( role == Qt::ToolTipRole )
            {
                return row->getExpressionError();
            }
        }
    }
    else if( index.column() == 1 )
    {
        if( role == Qt::EditRole )
        {
            return row->actionString();
        }

        if( !row->isActionOk())
        {
            if( role == Qt::BackgroundColorRole )
            {
                return Qt::red;
            }
            if( role == Qt::ToolTipRole )
            {
                return row->getActionError();
            }
        }

        if( row->getActionDisplayer() )
        {
            return row->getActionDisplayer()->toDisplay( role );
        }
        else if (role == Qt::DisplayRole )
        {
            return row->actionString();
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
        row->expressionAsString( value.toString() );
        return true;
    }
    else if( index.column() == 1 )
    {
        row->actionString( value.toString() );
        return true;
    }

    return false;
}
