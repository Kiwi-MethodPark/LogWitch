/*
 * FilterRuleRaw.cpp
 *
 *  Created on: Jun 10, 2011
 *      Author: sven
 */

#include "FilterRuleRaw.h"

FilterRuleRaw::FilterRuleRaw()
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

void FilterRuleRaw::actionString( const QString &act)
{
    m_actionAsString = act;
    m_actionParser.parse( m_actionAsString);
    emit changed();
}

TconstSharedDisplayItemData FilterRuleRaw::getActionDisplayer() const
{
    return m_actionParser.get();
}

const QString &FilterRuleRaw::actionString() const
{
    return m_actionAsString;
}


bool FilterRuleRaw::isActionOk() const
{
    return m_actionParser.isValid();
}


const QString &FilterRuleRaw::getActionError() const
{
    return m_actionParser.getError();
}

TSharedAction FilterRuleRaw::getCompiledAction() const
{
    return m_actionParser.get();
}
