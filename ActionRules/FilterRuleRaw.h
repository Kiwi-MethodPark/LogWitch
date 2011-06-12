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

    /// This is the action in the common string notation.
    QString actionAsString;

    bool actionOk;

    /**
     * This holds an action drawer which knows how to draw the current
     * actionAsString in a more human readable way.
     */
    TconstSharedDisplayItemData actionDisplayer;

signals:
    void changed ();

private:
    /// This is the expression in the common string notation.
    QString m_expressionAsString;

    ExpressionParser m_expressionParser;

};

#endif /* FILTERRULERAW_H_ */
