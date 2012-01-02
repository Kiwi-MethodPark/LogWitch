/*
 * ContextMenuLogEntryHeader.cpp
 *
 *  Created on: Dec 29, 2011
 *      Author: sven
 */

#include "ContextMenuLogEntryHeader.h"

#include <boost/bind.hpp>

#include "GUITools/SlotToBoostFunction.h"

ContextMenuLogEntryHeader::ContextMenuLogEntryHeader( QHeaderView *parent )
: QMenu( parent )
, m_header( parent )
{
    m_hideAction = addAction ( tr("hide") );
    m_showMenu = addMenu( tr("show") );

    QObject::connect(m_header, SIGNAL(customContextMenuRequested(const QPoint &)),
                     this, SLOT(contextMenuRequest(const QPoint &)));

    QObject::connect(m_hideAction, SIGNAL( triggered() ),
                     this, SLOT( hideColumn() ) );
}

void ContextMenuLogEntryHeader::contextMenuRequest( const QPoint & pos )
{
    m_headerToWorkOn = m_header->logicalIndexAt( pos );

    // Configure menu ....
    m_showMenu->clear();

    for( int i = 0; i < m_header->model()->columnCount(); i++ )
    {
        if( m_header->isSectionHidden( i ) )
        {
            QVariant v = m_header->model()->headerData( i, Qt::Horizontal, Qt::DisplayRole );
            QString name = v.toString();
            QAction *action = m_showMenu->addAction( name );

            connect( action,SIGNAL(triggered()),
                    // Destruction of this will be handled by the action itself.
                    new SlotToBoostFunction(action,boost::bind(&ContextMenuLogEntryHeader::showColumn,this,i)),
                    SLOT(handleSignal()));
        }
    }

    m_showMenu->setEnabled( !m_showMenu->isEmpty() );

    m_hideAction->setEnabled( m_header->hiddenSectionCount() < m_header->count() - 1 );

    popup( m_header->mapToGlobal( pos ) );
}

void ContextMenuLogEntryHeader::hideColumn()
{
    // Ensure at least one column is not hidden ....
    if( m_header->hiddenSectionCount() < m_header->count() - 1 )
        m_header->hideSection( m_headerToWorkOn );
}

void ContextMenuLogEntryHeader::showColumn( int i )
{
    m_header->showSection( i );
}
