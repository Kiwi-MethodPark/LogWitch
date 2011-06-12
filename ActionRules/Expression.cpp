/*
 * Expression.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#include "Expression.h"

Expression::~Expression()
{
}

std::ostream& operator<< (std::ostream &o, const Expression &e)
{
    return e.out(o);
}
