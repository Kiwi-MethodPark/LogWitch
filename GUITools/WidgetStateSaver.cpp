/*
 * WidgetStateSaver.cpp
 *
 *  Created on: May 28, 2011
 *      Author: sven
 */

#include "WidgetStateSaver.h"
#include <QtCore/QtCore>

WidgetStateSaver::WidgetStateSaver( QObject *parent )
    : QObject( parent )
    , m_lastObject( NULL )
{

}

WidgetStateSaver::~WidgetStateSaver()
{}

void WidgetStateSaver::addElementToWatch( QObject *obj,
        boost::shared_ptr<ObjectStateSavingInterface> stateSaver )
{
    qDebug() << "adding object to watch " <<obj;
    m_myWatchedObjects.insert( ObjectStateDumper::value_type( obj, stateSaver ) );
    // Save actual obj state as state default.
    StateSaveMap::iterator it = m_stateHistoryMap.find( NULL );
    if( it == m_stateHistoryMap.end() )
    {
        it = m_stateHistoryMap.insert(StateSaveMap::value_type( (QObject *) NULL, ObjectStateList() ) ).first;
    }

    it->second.push_back( stateSaver->dumpState( obj, NULL ) );
}

void WidgetStateSaver::storeState( QObject *obj )
{
    qDebug() << "Storing state";
    ObjectStateDumper::iterator it;
    ObjectStateList stateList;

    for( it = m_myWatchedObjects.begin(); it != m_myWatchedObjects.end(); ++it )
    {
        stateList.push_back( it->second->dumpState( it->first, m_lastObject ) );
    }

    m_stateHistoryMap.insert( StateSaveMap::value_type(obj, stateList ) );
}

void WidgetStateSaver::replayState( QObject *obj )
{
    StateSaveMap::iterator it;

    it = m_stateHistoryMap.find( obj );

    if( it != m_stateHistoryMap.end() )
    {
        qDebug() << "Replaying state";

        ObjectStateList::iterator itStates;
        for( itStates = it->second.begin(); itStates != it->second.end(); ++itStates )
        {
            (*itStates)->replayState( m_lastObject );
        }
    }
}

void WidgetStateSaver::deregisterFocusObject( QObject *end, bool applyDefauls )
{
    qDebug() << "DeregisterFocusObject" << end;
    if( applyDefauls )
        replayState( NULL );

    m_stateHistoryMap.erase(end );
}

void WidgetStateSaver::switchState( QObject *newObject)
{
    qDebug() << "Request for wwitch state from " << m_lastObject << " to new " << newObject;
    if( m_lastObject == newObject )
        return;
    qDebug() << "Doing Switch state from " << m_lastObject << " to new " << newObject;

    if( m_lastObject )
        storeState( m_lastObject );
    m_lastObject = newObject;
    replayState( newObject );
}
