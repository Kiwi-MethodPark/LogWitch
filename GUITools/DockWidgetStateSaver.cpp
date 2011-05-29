/*
 * DockWidgetStateSaver.cpp
 *
 *  Created on: May 28, 2011
 *      Author: sven
 */

#include "DockWidgetStateSaver.h"
#include <QtGui>
#include <QtCore/QtCore>

boost::shared_ptr<DockWidgetStateSaver> DockWidgetStateSaver::generate()
{
    shared_ptr<DockWidgetStateSaver> obj( new DockWidgetStateSaver() );
    obj->m_ptrToMyself = obj;

    return obj;
}

DockWidgetStateSaver::DockWidgetStateSaver()
{
}

boost::shared_ptr<ObjectState> DockWidgetStateSaver::dumpState( QObject *obj, QObject * ) const
{
    qDebug() << "dumping state" << obj;

    QDockWidget *wi = dynamic_cast<QDockWidget *>(obj);
    if( wi )
    {
        qDebug() << "Saving old widget: " << wi->widget();
        return boost::shared_ptr<ObjectState>(new state( m_ptrToMyself.lock(), obj, wi->widget() ));
    }
    else
    {
        qDebug() << "Ignoring: Cast failed";
        return boost::shared_ptr<ObjectState>(new ObjectState());
    }
}

void DockWidgetStateSaver::replayState( QObject *obj, QObject *, const ObjectState *stateP ) const
{
    qDebug() << "Replaying state" << obj;

    QDockWidget *wi = dynamic_cast<QDockWidget *>(obj);
    const state *st = dynamic_cast<const state *>(stateP);

    if( wi && st )
    {
        qDebug() << "Replaying old widget: " << wi->widget();
        wi->setWidget( st->m_dockWidget );
    }
    else
        qDebug() << "Ignoring: Cast failed";
}
