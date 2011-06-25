/*
 * Action.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#include "Action.h"

Action::Action()
{
}

Action::~Action()
{
}

bool Action::modifyData( QVariant &,  int , int ) const
{
    return false;
}

bool Action::isValid() const
{
    return true;
}
