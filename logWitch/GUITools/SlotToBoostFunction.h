/*
 * SlotToBoostFunction.h
 *
 *  Created on: Dec 30, 2011
 *      Author: sven
 */

#ifndef SLOTTOBOOSTFUNCTION_H_
#define SLOTTOBOOSTFUNCTION_H_
#include <QtCore>

#include <boost/function.hpp>

class SlotToBoostFunction
: public QObject
{
    Q_OBJECT
public:
    SlotToBoostFunction( QObject * parent, boost::function<void(void)> const & func):
        QObject( parent ), m_func( func )
    {  }

public slots:
    void handleSignal()
    {
        m_func();
    }

private:
  boost::function<void(void)> m_func;
};

class SlotToBoostFunction_int_int_int
: public QObject
{
    Q_OBJECT
public:
    SlotToBoostFunction_int_int_int( QObject * parent, boost::function<void(int,int,int)> const & func):
        QObject( parent ), m_func( func )
    {  }

public slots:
    void handleSignal(int a, int b, int c)
    {
        m_func(a,b,c);
    }

private:
  boost::function<void(int,int,int)> m_func;
};

#endif /* SLOTTOBOOSTFUNCTION_H_ */
