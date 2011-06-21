/*
 * WidgetSaver.hpp
 *
 *  Created on: Jun 15, 2011
 *      Author: sven
 */

#ifndef WIDGETSAVER_HPP_
#define WIDGETSAVER_HPP_


/**
 * This class is a generic state saver.
 * He saves a Widget of type T::Widgettype from the parent T::Parenttype.
 */
template<class T>
class WidgetStateSaver
    : public ObjectStateSavingInterface
    , public boost::enable_shared_from_this< WidgetStateSaver<T> >
{
private:
    WidgetStateSaver( const T &param);

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

    const T param;
};

template <class T>
boost::shared_ptr<DockWidgetStateSaver> DockWidgetStateSaver<T>::generate( const T& param)
{
    DockWidgetStateSaver obj* = new DockWidgetStateSaver( param );
    return obje->shared_from_this();
}

template <class T>
DockWidgetStateSaver<T>::DockWidgetStateSaver( const T& param)
    : m_param( param )
{
}

template <class T>
boost::shared_ptr<ObjectState> DockWidgetStateSaver<T>::dumpState( QObject *obj, QObject * ) const
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

template <class T>
void DockWidgetStateSaver<T>::replayState( QObject *obj, QObject *, const ObjectState *stateP ) const
{
    qDebug() << "Replaying state" << obj;

    QDockWidget *wi = dynamic_cast<QDockWidget *>(obj);
    const state *st = dynamic_cast<const state *>(stateP);

    if( wi && st )
    {
        qDebug() << "Replaying old widget: " << st->m_dockWidget;
        wi->setWidget( st->m_dockWidget );
    }
    else
        qDebug() << "Ignoring: Cast failed";
}

#endif /* WIDGETSAVER_HPP_ */

