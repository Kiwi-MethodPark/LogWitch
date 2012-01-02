/*
 * ValueGetter.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#include "ValueGetter.h"

ValueGetter::~ValueGetter()
{
}

std::ostream& operator<< (std::ostream &o, const ValueGetter &e)
{
    return e.out(o);
}
