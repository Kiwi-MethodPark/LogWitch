/*
 * FilterRuleCompiled.cpp
 *
 *  Created on: Jun 10, 2011
 *      Author: sven
 */

#include "FilterRuleCompiled.h"

// take the string and search for => as an special expression.
static const QString seperator("=>");

FilterRuleCompiled::FilterRuleCompiled( const QString &str, TSharedConstLogEntryParserModelConfiguration cfg )
: m_expression( cfg )
, m_action( cfg )
{
    // Check if we can parse the string ...
    QStringList vals = str.split(seperator);

    if( vals.size() == 1 )
    {
        expressionString( vals.front() );
    }
    else if( vals.size() == 2 )
    {
        expressionString( vals.at(0) );
        actionString( vals.at(1) );
    }

    parseRule();
}

FilterRuleCompiled::~FilterRuleCompiled()
{
}

TSharedExpression FilterRuleCompiled::getExpression() const
{
    return m_expression.get();
}

bool FilterRuleCompiled::validWithinContext() const
{
    return m_expression.isValid() && m_expression.get()->isValid()
            && m_action.isValid() && m_action.get()->isValid();
}

void FilterRuleCompiled::parseRule()
{
    m_action.parse( m_actionAsString );
    m_expression.parse( m_expressionAsString );
    if( m_expression.isValid() && m_action.isValid() )
    {
        m_compiledRule.reset( new Rule( m_expression.get(), m_action.get() ) );
    }
    else
        m_compiledRule.reset();

    emit changed();
}

TSharedRule FilterRuleCompiled::getCompiledRule()
{
    return m_compiledRule;
}

QString FilterRuleCompiled::toString() const
{
    return expressionString() + seperator + actionString();
}

void FilterRuleCompiled::expressionString( const QString &exp )
{
    m_expressionAsString = exp;
    m_expression.parse( m_expressionAsString );
    parseRule();
}

const QString &FilterRuleCompiled::expressionString( ) const
{
    return m_expressionAsString;
}

const QString &FilterRuleCompiled::getExpressionError() const
{
    return m_expression.getError();
}

bool FilterRuleCompiled::isExpressionOk() const
{
    return m_expression.isValid();
}

void FilterRuleCompiled::actionString( const QString &act)
{
    m_actionAsString = act;
    m_action.parse( m_actionAsString);
    parseRule();
}

TconstSharedDisplayItemData FilterRuleCompiled::getActionDisplayer() const
{
    return m_action.get();
}

const QString &FilterRuleCompiled::actionString() const
{
    return m_actionAsString;
}


bool FilterRuleCompiled::isActionOk() const
{
    return m_action.isValid();
}


const QString &FilterRuleCompiled::getActionError() const
{
    return m_action.getError();
}
