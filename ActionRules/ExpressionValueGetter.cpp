/*
 * ExpressionValueGetter.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#include "ExpressionValueGetter.h"
#include "ValueGetter.h"

ExpressionValueGetter::ExpressionValueGetter( TconstSharedValueGetter left, TconstSharedValueGetter right )
    : m_left( left )
    , m_right( right )
{
}

ExpressionValueGetter::~ExpressionValueGetter()
{

}

bool ExpressionValueGetter::match( TconstSharedLogEntry &entry ) const
{
    return *(m_left->getValue( entry )) == *(m_right->getValue( entry ));
}

bool ExpressionValueGetter::isValid( ) const
{
    return m_left->isValid() && m_right->isValid();
}
