/*
 * ActionRemoveRow.h
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#ifndef ACTIONREMOVEROW_H_
#define ACTIONREMOVEROW_H_
#include "ActionRules/Action.h"

class ActionRemoveRow
    : public Action
{
public:
    ActionRemoveRow();
    ~ActionRemoveRow();

    QVariant toDisplay( int role ) const;

    void execute( ) const {};
};

#endif /* ACTIONREMOVEROW_H_ */
