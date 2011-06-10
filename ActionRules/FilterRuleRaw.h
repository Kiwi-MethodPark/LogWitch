/*
 * FilterRuleRaw.h
 *
 *  Created on: Jun 10, 2011
 *      Author: sven
 */

#ifndef FILTERRULERAW_H_
#define FILTERRULERAW_H_
#include <boost/shared_ptr.hpp>
#include <QtCore/QtCore>

#include "ActionRules/DisplayItemData.h"

class FilterRuleRaw;
typedef boost::shared_ptr<FilterRuleRaw> TSharedFilterRuleRaw;
typedef boost::shared_ptr<const FilterRuleRaw> TSharedConstFilterRuleRaw;

/**
 * This is a FilterRule in a more abstract representation which is also human readable.
 * Before this rule can be used it must be compiled.
 */
class FilterRuleRaw
{
public:
    FilterRuleRaw();
    ~FilterRuleRaw();

    /// This is the expression in the common string notation.
    QString expressionAsString;

    /// This is the action in the common string notation.
    QString actionAsString;

    /**
     * This holds an action drawer which knows how to draw the current
     * actionAsString in a more human readable way.
     */
    TconstSharedDisplayItemData actionDisplayer;
};

#endif /* FILTERRULERAW_H_ */
