/*
 * FilterRuleRaw.cpp
 *
 *  Created on: Jun 10, 2011
 *      Author: sven
 */

#include "FilterRuleRaw.h"

FilterRuleRaw::FilterRuleRaw()
: actionOk( false )
{
}

FilterRuleRaw::~FilterRuleRaw()
{
}

void FilterRuleRaw::expressionAsString( const QString &exp )
{
    m_expressionAsString = exp;
    m_expressionParser.parse( m_expressionAsString );
    emit changed();
}

const QString &FilterRuleRaw::expressionAsString( ) const
{
    return m_expressionAsString;
}

const QString &FilterRuleRaw::getExpressionError() const
{
    return m_expressionParser.getError();
}

bool FilterRuleRaw::isExpressionOk() const
{
    return m_expressionParser.isValid();
}

