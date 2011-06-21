/*
 * FilterRuleCompiled.h
 *
 *  Created on: Jun 10, 2011
 *      Author: sven
 */
#ifndef FILTERRULECOMPILED_H_
#define FILTERRULECOMPILED_H_
#include <boost/shared_ptr.hpp>

#include "ActionRules/Action.h"
#include "ActionRules/Expression.h"
#include "ActionRules/FilterRuleRaw.h"
#include "ActionRules/Rule.h"

class FilterRuleCompiled;
typedef boost::shared_ptr<FilterRuleCompiled> TSharedFilterRuleCompiled;
typedef boost::shared_ptr<const FilterRuleCompiled> TSharedConstFilterRuleCompiled;

class FilterRuleCompiled
: public QObject
{
    Q_OBJECT
public:
    FilterRuleCompiled(TSharedConstFilterRuleRaw desc, TSharedConstLogEntryParserModelConfiguration cfg );
    ~FilterRuleCompiled();

    /**
     *  If the expression is valid within the current
     * context, getExpression returns the expression.
     */
    TSharedExpression getExpression() const;

    /**
     * Return true if the rule is compileable and valid within the current context.
     */
    bool validWithinContext() const;

    /**
     * If the action is compileable within the current context
     * this holds the compiled action.
     * At the moment an action is always compileable, so this
     * can be used also for rendering the action for the display.
     *
     * Actually actions does not need any context.
     */
    // TconstSharedAction compiledAction;

    TSharedConstFilterRuleRaw getDescription() const;

    /// This is the complete rule bounded to a given context.
    TSharedRule getCompiledRule();

signals:
    void changed();

protected slots:
    void parseRule();

private:
    /**
     *  This is the parent filtering rule (the parent is always the rule from the pool).
     *  This pool holds all rules and from this pool a rule will be compiled to the
     *  different contexts.
     */
    TSharedConstFilterRuleRaw m_rawRule;

    ExpressionParser m_expression;

    TSharedRule m_compiledRule;
};

#endif /* FILTERRULECOMPILED_H_ */
