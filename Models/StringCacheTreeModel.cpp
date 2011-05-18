/*
 * StringCacheTreeModel.cpp
 *
 *  Created on: May 15, 2011
 *      Author: sven
 */

#include "StringCacheTreeModel.h"

#include <iostream>
#include <QtCore/QtCore>

StringCacheTreeModel::StringCacheTreeModel( const StringCache * cache, const QString &splitString  )
	: m_rootNode( new StringCacheTreeItem( TSharedConstQString(new QString("RootNode")) ) )
	, m_splitRegex( )
{
	if( !splitString.isEmpty() )
		m_splitRegex.reset( new QRegExp( splitString ) );

    QObject::connect(cache, SIGNAL(newStringElement( TSharedConstQString )),
                     this, SLOT(newStringElement( TSharedConstQString ) ) );
}

StringCacheTreeModel::~StringCacheTreeModel()
{

}



QVariant StringCacheTreeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    StringCacheTreeItem *item = static_cast<StringCacheTreeItem*>(index.internalPointer());

	if( role == Qt::CheckStateRole )
	{
		if( item->getForcedChecked() )
			return QVariant();

		return item->getChecked();
	}
	else if (role == Qt::DisplayRole)
		return *(item->getString());


    return QVariant();
}

bool StringCacheTreeModel::setData( const QModelIndex &index, const QVariant& value, int role )
{
	qDebug() << "setData called with " << value;
	if( !index.isValid())
		return false;

	if( role != Qt::CheckStateRole )
	  return false;

	StringCacheTreeItem *item = static_cast<StringCacheTreeItem*>( index.internalPointer() );
	item->nextChecked();


	QModelIndex node = index;
	while( node.isValid() )
	{
		emit dataChanged( node, node );
		node = node.parent();
	}

   	dataChangedToChildren(index);

	// dataChanged( top, bottom );

	return true;
}

void StringCacheTreeModel::dataChangedToChildren(const QModelIndex &index )
{
	emit dataChanged(index, index);
	if( !hasChildren(index) )
		return;

	for(int childRow = 0; childRow < rowCount(index); ++childRow)
	{
	    QModelIndex childIndex = index.child(childRow, 0);
	    dataChangedToChildren( childIndex );
	}
}

Qt::ItemFlags StringCacheTreeModel::flags(const QModelIndex &/* index*/ ) const
{

	return /*QAbstractItemModel::flags( index ) | */ Qt::ItemIsTristate | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;

}

//QVariant StringCacheTreeModel::headerData(int section, Qt::Orientation orientation,
//                    int role = Qt::DisplayRole) const
//{
//
//}

QModelIndex StringCacheTreeModel::index(int row, int column,
                  const QModelIndex &parent /*= QModelIndex()*/) const
{
	 if (!hasIndex(row, column, parent))
		 return QModelIndex();

	 StringCacheTreeItem *parentItem;

	 if (!parent.isValid())
		 parentItem = m_rootNode.get();
	 else
		 parentItem = static_cast<StringCacheTreeItem*>(parent.internalPointer());

	 StringCacheTreeItem *childItem = parentItem->child(row);
	 if (childItem)
		 return createIndex(row, column, childItem);
	 else
		 return QModelIndex();
}

QModelIndex StringCacheTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    StringCacheTreeItem *childItem = static_cast<StringCacheTreeItem*>(index.internalPointer());
    StringCacheTreeItem *parentItem = childItem->parent();

    if (parentItem == m_rootNode.get())
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);
}

int StringCacheTreeModel::rowCount(const QModelIndex &parent /* = QModelIndex() */) const
{
    StringCacheTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = m_rootNode.get();
    else
        parentItem = static_cast<StringCacheTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int StringCacheTreeModel::columnCount(const QModelIndex &/* parent = QModelIndex()*/ ) const
{
	return 1;
}

void StringCacheTreeModel::newStringElement( TSharedConstQString string )
{
	//qDebug() << "Received new string: " << *string ;
	if( m_splitRegex )
	{
		QStringList list = string->split( *m_splitRegex );
		StringCacheTreeItem *node = m_rootNode.get();
		QModelIndex idx;

		while( !list.empty() )
		{
			QString str = list.front();
			list.pop_front();

			int i = 0;
			const int count = node->childCount();
			for( ; i < count; i++ )
			{
				if( *(node->child(i)->getString()) == str )
				{
					node = node->child(i);
					idx = index(i,0,idx);
					break;
				}
			}

			if( i == count )
			{
				beginInsertRows( idx, node->childCount(), node->childCount() );
				StringCacheTreeItem *newNode = new StringCacheTreeItem( TSharedConstQString( new QString(str) ), node );
				node->appendChild( newNode );
				node = newNode;
				idx = index(i,0,idx);
				endInsertRows();
			}
		}
	}
	else
	{
		beginInsertRows( QModelIndex(), m_rootNode->childCount(), m_rootNode->childCount() );
		m_rootNode->appendChild( new StringCacheTreeItem( string, m_rootNode.get() ) );
		endInsertRows();
	}
}

