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
    QSettings settings;

    const int size = settings.beginReadArray("Rules");
    for (int i = 0; i < size; ++i)
    {
         settings.setArrayIndex(i);

         TSharedFilterRuleRaw row = TSharedFilterRuleRaw( new FilterRuleRaw(settings.value("rule").toString()) );
         m_table.push_back( row );
    }

    settings.endArray();
}

TableModelRules::~TableModelRules()
{
    QSettings settings;
    settings.beginWriteArray("Rules");
    for (unsigned int i = 0; i < m_table.size(); ++i) {
        settings.setArrayIndex(i);
        settings.setValue("rule", m_table[i]->toString() );
    }
    settings.endArray();
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

Qt::ItemFlags TableModelRules::flags(const QModelIndex &index  ) const
{
    Qt::ItemFlags defaultFlags = QAbstractTableModel::flags(index);

     if (index.isValid())
         return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable
              | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
     else
         return Qt::ItemIsDropEnabled | defaultFlags;
}

Qt::DropActions TableModelRules::supportedDropActions() const
{
     return Qt::CopyAction;
}

QStringList TableModelRules::mimeTypes() const
{
    QStringList types;
    types << QLatin1String("application/x-de.steckmann.LogWitch.rule");
    types << QLatin1String("text/plain");
    return types;
}


bool TableModelRules::dropMimeData(const QMimeData *data,
    Qt::DropAction action, int row, int column, const QModelIndex &parent)
{
    if (action == Qt::IgnoreAction)
        return true;

    int insertPos = (parent.row() < 0) ? m_table.size() : parent.row();
    if( insertPos >= m_table.size() )
        insertPos = m_table.size();

    QStringList formats = data->formats();

    for( QStringList::const_iterator it = formats.begin(); it != formats.end(); ++it )
    {
        if( *it == mimeTypes().front() )
        {
            const QByteArray id = getIdentification();
            QByteArray dataArr = data->data( *it );
            QDataStream stream( &dataArr, QIODevice::ReadOnly );

            // We will move data here, because this data is comming from us!
            if( dataArr.startsWith(id ) )
            {
                dataArr.remove( 0, id.length() );
                std::list<int> srcRows;
                while( !stream.atEnd() )
                {
                    int row;
                    stream >> row;
                    srcRows.push_back( row );
                }

                srcRows.sort();

                // Now extract the rows and place them to the new position.
                std::list<TSharedFilterRuleRaw> rules;
                for( std::list<int>::reverse_iterator it = srcRows.rbegin(); it != srcRows.rend(); ++it )
                {
                    int rowMapped = *it;
                    if( rowMapped < m_table.size() && rowMapped >= 0)
                    {
                        rules.push_front( m_table[rowMapped] );
                        m_table.erase( m_table.begin() + rowMapped );
                        if( rowMapped <= insertPos )
                        {
                            if( insertPos > 0 )
                                insertPos--;
                        }
                    }
                }

                for( std::list<TSharedFilterRuleRaw>::reverse_iterator it = rules.rbegin()
                    ; it != rules.rend()
                    ; ++it )
                {
                    m_table.insert( m_table.begin() + insertPos, *it );
                }

                reset();
                return true;
            }
        }
    }

    if( data->hasText() )
    {
        QString ruleText = data->text();
        QStringList singleRuleTexts = ruleText.split( '\n' );

        for( QStringList::iterator it = singleRuleTexts.begin()
            ; it != singleRuleTexts.end()
            ; ++it )
        {
            TSharedFilterRuleRaw rule = TSharedFilterRuleRaw( new FilterRuleRaw( *it ) );

            if( rule->isActionOk() && rule->isExpressionOk() )
            {
                beginInsertRows( QModelIndex(), insertPos, insertPos );
                m_table.insert( m_table.begin() + insertPos, rule );
                endInsertRows();
                insertPos++;
            }
        }

        return true;
    }

    return false;
}

QByteArray TableModelRules::getIdentification() const
{
    QByteArray encSource;
    QDataStream srcStream(&encSource, QIODevice::WriteOnly);
    srcStream << QCoreApplication::applicationPid() << (const void *)this;
    return encSource;
}

QMimeData *TableModelRules::mimeData(const QModelIndexList &indexes) const
{
    if (indexes.count() <= 0)
        return 0;
    QStringList types = mimeTypes();
    if (types.isEmpty())
        return 0;
    QMimeData *data = new QMimeData();
    QString format = types.at(0);

    QString encoded;
    QTextStream stream(&encoded, QIODevice::WriteOnly);

    QByteArray encSource;
    QDataStream srcStream(&encSource, QIODevice::WriteOnly);

    std::list<int> rowSet;

    QModelIndexList::ConstIterator it = indexes.begin();
    bool lEnd = false;
    for (; it != indexes.end(); ++it)
    {
        const int row = (*it).row();
        if( find( rowSet.begin(), rowSet.end(), row ) == rowSet.end() )
        {
            rowSet.push_back( row );
            if( row >= 0 && row < m_table.size() )
            {
                if( lEnd )
                    stream << '\n';
                stream << m_table[row]->toString();
                lEnd = true;

                srcStream << row;
            }
        }
    }

    data->setData( format, encSource.prepend( getIdentification() ) );
    data->setText( encoded );
    return data;
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
