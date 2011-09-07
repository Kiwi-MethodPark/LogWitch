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
#include "ActionRules/ExpressionParser.h"
#include "ActionParser.h"

class FilterRuleRaw;
typedef boost::shared_ptr<FilterRuleRaw> TSharedFilterRuleRaw;
typedef boost::shared_ptr<const FilterRuleRaw> TSharedConstFilterRuleRaw;

/**
 * This is a FilterRule in a more abstract representation which is also human readable.
 * Before this rule can be used it must be compiled.
 */
class FilterRuleRaw
: public QObject
{
    Q_OBJECT
public:
    FilterRuleRaw();
    FilterRuleRaw( const QString &str );
    ~FilterRuleRaw();

    /**
     * Sets an expression string and parses him.
     */
    void expressionAsString( const QString &exp );
    /**
     * Retrieves the current expresseion string.
     */
    const QString &expressionAsString( ) const;

    /**
     * Returns true if the current set expression is compileable.
     */
    bool isExpressionOk() const;

    /**
     * Returns the error from the parser if isExpressionOk returns false.
     */
    const QString &getExpressionError() const;

    /**
     * Sets a new string for an action.
     */
    void actionString( const QString &act);

    /**
     * Retrieve the string for the action.
     */
    const QString &actionString() const;

    /**
     * Returns true if actionString is parseable to an Action object.
     */
    bool isActionOk() const;

    /**
     * If isActionOk is false, this function retrieves the error.
     */
    const QString &getActionError() const;

    /**
     * This holds an action drawer which knows how to draw the current
     * actionAsString in a more human readable way.
     */
    TconstSharedDisplayItemData getActionDisplayer() const;

    QString toString() const;

signals:
    void changed ();

private:
    /// This is the expression in the common string notation.
    QString m_expressionAsString;

    ExpressionParser m_expressionParser;

    /// This is the action in the common string notation.
    QString m_actionAsString;

    ActionParser m_actionParser;
};

#endif /* FILTERRULERAW_H_ */
