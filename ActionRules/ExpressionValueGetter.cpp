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

ExpressionValueGetter::ExpressionValueGetter( TconstSharedValueGetter left )
    : m_left( left )
    , m_right(  )
{
}

ExpressionValueGetter::~ExpressionValueGetter()
{

}

void ExpressionValueGetter::setRight( TconstSharedValueGetter right )
{
    m_right = right;
}

bool ExpressionValueGetter::match( TconstSharedLogEntry &entry ) const
{
    return *(m_left->getValue( entry )) == *(m_right->getValue( entry ));
}

bool ExpressionValueGetter::isValid( ) const
{
    return m_left && m_left->isValid() && m_right && m_right->isValid();
}

std::ostream &ExpressionValueGetter::out( std::ostream &o, bool extended ) const
{
    if( extended )
    {
        o << "ExprVG{";
        m_left->out( o, extended );
        o << " == ";
        m_right->out( o, extended );
        o << "}";
    }
    else
    {
        o << *m_left << " == " << *m_right;
    }

    return o;
}
