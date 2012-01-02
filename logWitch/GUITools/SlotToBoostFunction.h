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

#endif /* SLOTTOBOOSTFUNCTION_H_ */
