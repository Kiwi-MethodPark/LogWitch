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

    /**
     * With this the data row can be modified.
     *
     * @param var incoming original data to modify or leave.
     * @param column number of the column the data is from
     * @param role the role which this var is for.
     * @return returns true if var was modified, otherwise false.
     */
    virtual bool modifyData( QVariant &var,  int column, int role) const;
};

typedef boost::shared_ptr<Action> TSharedAction;
typedef boost::shared_ptr<const Action> TconstSharedAction;


#endif /* ACTION_H_ */
