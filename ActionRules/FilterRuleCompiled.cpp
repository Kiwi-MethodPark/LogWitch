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
    return m_expression.isValid() && m_expression.get()->isValid();
}

void FilterRuleCompiled::parseRule()
{
    m_expression.parse( m_rawRule->expressionAsString() );
    emit changed();
}
TSharedConstFilterRuleRaw FilterRuleCompiled::getDescription() const
{
    return m_rawRule;
}


