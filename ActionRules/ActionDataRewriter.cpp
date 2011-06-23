/*
 * ActionColorRow.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#include "ActionDataRewriter.h"

#include <QtGui>

#include "LogEntryParserModelConfiguration.h"
#include "LogData/LogEntryAttributeFactory.h"

ActionDataRewriter::ActionDataRewriter(TSharedConstLogEntryParserModelConfiguration configuration)
: m_cfg( configuration )
{
}

ActionDataRewriter::~ActionDataRewriter()
{
}

bool ActionDataRewriter::modifyData( QVariant &var,  int column, int role) const
{
    TChangeSet::const_iterator it = m_changes.find( rc_key(role,column) );
    if( it == m_changes.end() )
        it = m_changes.find( rc_key(role) );


    if( it != m_changes.end() )
    {
        var = it->second;
        return true;
    }

    return false;
}

QVariant ActionDataRewriter::toDisplay( int role ) const
{
    QVariant variant;

    if( role == Qt::DisplayRole )
    {
        variant = QString( QObject::tr("Text changing") );
    }

    TChangeSet::const_iterator it = m_changes.find( rc_key(role) );
    if( it != m_changes.end() )
    {
        variant = it->second;
    }
    else
    {
        for( it = m_changes.begin(); it != m_changes.end(); ++it )
        {
            if( it->first.role == role )
            {
                variant = it->second;
            }
        }
    }

    return variant;
}

void ActionDataRewriter::addChangeSet( const QVariant &var, int role )
{
    m_changes.insert( TChangeSet::value_type(rc_key(role),var) );
}

void ActionDataRewriter::addChangeSet( const QVariant &var, int role, const QString &column )
{
    if( m_cfg )
    {
        int fieldCount = m_cfg->getLogEntryAttributeFactory()->getNumberOfFields();
        for( int i = 0; i < fieldCount; i++ )
        {
            if( column == m_cfg->getLogEntryAttributeFactory()->getDescShort( i ) )
            {
                m_changes.insert( TChangeSet::value_type(rc_key(role, i),var) );
                break;
            }
        }
    }
}

