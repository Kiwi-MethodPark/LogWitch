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
private:
    TconstSharedValueGetter m_left;
    TconstSharedValueGetter m_right;
};

#endif /* EXPRESSIONVALUEGETTER_H_ */
