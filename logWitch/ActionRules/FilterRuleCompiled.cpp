/*
 * FilterRuleCompiled.cpp
 *
 *  Created on: Jun 10, 2011
 *      Author: sven
 */

#include "FilterRuleCompiled.h"

FilterRuleCompiled::FilterRuleCompiled( TSharedConstFilterRuleRaw desc, TSharedConstLogEntryParserModelConfiguration cfg )
: m_rawRule( desc )
, m_expression( cfg )
, m_action( cfg )
{
    parseRule();

    QObject::connect(m_rawRule.get(), SIGNAL(changed()),
                     this, SLOT(parseRule()));
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
    m_action.parse( m_rawRule->actionString() );
    m_expression.parse( m_rawRule->expressionAsString() );
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

TSharedConstFilterRuleRaw FilterRuleCompiled::getDescription() const
{
    return m_rawRule;
}


