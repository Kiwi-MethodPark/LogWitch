/*
 * SignalMultiplexerStateApplier.h
 *
 *  Created on: May 29, 2011
 *      Author: sven
 */

#ifndef SIGNALMULTIPLEXERSTATEAPPLIER_H_
#define SIGNALMULTIPLEXERSTATEAPPLIER_H_

#include <boost/weak_ptr.hpp>

#include "WidgetStateSaver.h"

class QWidget;
class SignalMultiplexer;

class SignalMultiplexerStateApplier
    : public ObjectStateSavingInterface
{
private:
        SignalMultiplexerStateApplier( SignalMultiplexer *);

public:
    static boost::shared_ptr<SignalMultiplexerStateApplier> generate( SignalMultiplexer *);

public:
    boost::shared_ptr<ObjectState> dumpState( QObject *obj, QObject * ) const;
    void replayState( QObject *obj, QObject *, const ObjectState *state ) const;

private:

    boost::weak_ptr<SignalMultiplexerStateApplier> m_ptrToMyself;

    SignalMultiplexer *m_multiplexer;
};

#endif /* SIGNALMULTIPLEXERSTATEAPPLIER_H_ */
