/*
 * StringCacheTreeItem.cpp
 *
 *  Created on: May 15, 2011
 *      Author: sven
 */

#include "StringCacheTreeItem.h"

StringCacheTreeItem::StringCacheTreeItem( TSharedConstQString str, StringCacheTreeItem *parent /* = NULL */ )
	: m_parentItem( parent )
	, m_checked( Child )
	, m_string( str )
{

}

StringCacheTreeItem::~StringCacheTreeItem()
{
	qDeleteAll(m_childItems);
}

void StringCacheTreeItem::appendChild( StringCacheTreeItem *child)
{
	m_childItems.append( child );

}

StringCacheTreeItem* StringCacheTreeItem::child(int row)
{
	return m_childItems.value(row);
}

int StringCacheTreeItem::childCount() const
{
	return m_childItems.count();
}

int StringCacheTreeItem::row() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf( const_cast<StringCacheTreeItem*>(this) );

    return 0;
}

StringCacheTreeItem *StringCacheTreeItem::parent()
{
	return m_parentItem;
}

TSharedConstQString StringCacheTreeItem::getString() const
{
	return m_string;
}

