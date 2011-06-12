/*
 * ExpressionValueGetter.h
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#ifndef EXPRESSIONVALUEGETTER_H_
#define EXPRESSIONVALUEGETTER_H_
#include "Expression.h"
#include "ValueGetter.h"

class ExpressionValueGetter
    : public Expression
{
public:
    ExpressionValueGetter( TconstSharedValueGetter left, TconstSharedValueGetter right );
    ~ExpressionValueGetter();

    bool isValid( ) const;

    virtual bool match( TconstSharedLogEntry &entry ) const;

    std::ostream &out( std::ostream &o, bool extended = false ) const;
private:
    TconstSharedValueGetter m_left;
    TconstSharedValueGetter m_right;
};


typedef boost::shared_ptr<ExpressionValueGetter> TSharedExpressionValueGetter;
typedef boost::shared_ptr<const ExpressionValueGetter> TconstSharedExpressionValueGetter;

#endif /* EXPRESSIONVALUEGETTER_H_ */
