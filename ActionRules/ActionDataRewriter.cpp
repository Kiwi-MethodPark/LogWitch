/*
 * ActionColorRow.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#include "ActionDataRewriter.h"
#include <QtGui>

ActionDataRewriter::ActionDataRewriter()
{
}

ActionDataRewriter::~ActionDataRewriter()
{
}

bool ActionDataRewriter::getData( QVariant &variant, int role ) const
{
    TChangeSet::const_iterator it = m_changes.find( role );
    if( it != m_changes.end() )
    {
        variant = it->second;
        return true;
    }

    return false;
}

QVariant ActionDataRewriter::toDisplay( int role ) const
{
    QVariant variant;

    TChangeSet::const_iterator it = m_changes.find( role );
    if( it != m_changes.end() )
    {
        variant = it->second;
    }

    if( role == Qt::DisplayRole )
    {
        variant = QString( QObject::tr("Row coloring") );
    }

    return variant;
}

void ActionDataRewriter::addChangeSet( const QVariant &var, int role )
{
    m_changes.insert( TChangeSet::value_type(role,var) );
}

