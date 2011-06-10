/*
 * Action.h
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#ifndef ACTION_H_
#define ACTION_H_
#include <boost/shared_ptr.hpp>
#include <QtCore/QVariant>

#include "ActionRules/DisplayItemData.h"

/**
 * This is triggered by a specific incoming log message or new connection.
 */
class Action
    : public DisplayItemData
{
public:
    Action();
    virtual ~Action();

    virtual void execute( ) const = 0;
};

typedef boost::shared_ptr<Action> TSharedAction;
typedef boost::shared_ptr<const Action> TconstSharedAction;


#endif /* ACTION_H_ */
