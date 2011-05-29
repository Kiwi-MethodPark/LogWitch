/*
 * DockWidgetStateSaver.h
 *
 *  Created on: May 28, 2011
 *      Author: sven
 */

#ifndef DOCKWIDGETSTATESAVER_H_
#define DOCKWIDGETSTATESAVER_H_

#include <boost/weak_ptr.hpp>

#include "WidgetStateSaver.h"

class QWidget;

class DockWidgetStateSaver
	: public ObjectStateSavingInterface
{
private:
	DockWidgetStateSaver();

public:
	static boost::shared_ptr<DockWidgetStateSaver> generate();

public:
    boost::shared_ptr<ObjectState> dumpState( QObject *obj, QObject * ) const;
    void replayState( QObject *obj, QObject *, const ObjectState *state ) const;

private:
    class state:
        public ObjectState
    {
        friend class DockWidgetStateSaver;
    public:

        state( boost::shared_ptr<ObjectStateSavingInterface> ptr, QObject *dock, QWidget *widget ): ObjectState( ptr, dock ), m_dockWidget( widget ) { }
    private:
        QWidget *m_dockWidget;
    };

    boost::weak_ptr<DockWidgetStateSaver> m_ptrToMyself;
};

#endif /* DOCKWIDGETSTATESAVER_H_ */
