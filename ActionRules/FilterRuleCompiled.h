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
{
public:
    FilterRuleCompiled();
    ~FilterRuleCompiled();

    /**
     *  If the expression is valid within the current
     *  context (parser) this holds the compiled expression.
     */
    TSharedExpression compiledExpression;

    /**
     * If the action is compileable within the current context
     * this holds the compiled action.
     * At the moment an action is always compileable, so this
     * can be used also for rendering the action for the display.
     */
    TconstSharedAction compiledAction;

    /**
     *  This is the parent filtering rule (the parent is always the rule from the pool).
     *  This pool holds all rules and from this pool a rule will be compiled to the
     *  different contexts.
     */
    TSharedConstFilterRuleRaw parentRule;

    /// This is the complete rule bounded to a given context.
    TSharedRule compiledRule;

    /// This is an additional flag if this rule is used in the current context.
    bool isActive;
};

#endif /* FILTERRULECOMPILED_H_ */
