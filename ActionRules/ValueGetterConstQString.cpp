/*
 * ValueGetterConstQString.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#include "ValueGetterConstQString.h"

ValueGetterConstQString::ValueGetterConstQString( const QString &string)
    : m_string( new QString( string ) )
{

}

ValueGetterConstQString::ValueGetterConstQString( TSharedConstQString &string )
    : m_string( string )
{

}

ValueGetterConstQString::~ValueGetterConstQString()
{

}


TSharedConstQString ValueGetterConstQString::getValue( TconstSharedLogEntry & ) const
{
    return m_string;
}

std::ostream &ValueGetterConstQString::out( std::ostream &o, bool extended ) const
{
    if( extended )
    {
        o << "ValueGetterConstQString{"<< "\"" << m_string->toStdString() << "\"" << "}";
        return o;
    }
    else
    {
        o << "\"" << m_string->toStdString() << "\"";
        return o;
    }
}
