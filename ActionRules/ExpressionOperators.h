/*
 * ExpressionOperators.h
 *
 *  Created on: Jun 25, 2011
 *      Author: sven
 */

#ifndef EXPRESSIONOPERATORS_H_
#define EXPRESSIONOPERATORS_H_
#include "ActionRules/Expression.h"

class ExpressionOpNegate
    :public Expression
{
public:
    ExpressionOpNegate( TSharedExpression in);

    virtual bool isValid( ) const;

    virtual bool match( TconstSharedLogEntry &entry ) const;

    virtual std::ostream &out( std::ostream &o, bool extended = false ) const;
private:
    TSharedExpression m_expr;
};

typedef boost::shared_ptr<ExpressionOpNegate> TSharedExpressionOpNegate;
typedef boost::shared_ptr<const ExpressionOpNegate> TconstSharedExpressionOpNegate;

class ExpressionConst
    :public Expression
{
public:
    ExpressionConst( bool value );

    virtual bool isValid( ) const;

    virtual bool match( TconstSharedLogEntry &entry ) const;

    virtual std::ostream &out( std::ostream &o, bool extended = false ) const;
private:
    bool m_value;
};

typedef boost::shared_ptr<ExpressionConst> TSharedExpressionConst;
typedef boost::shared_ptr<const ExpressionConst> TconstSharedExpressionConst;

class ExpressionOpOr
    :public Expression
{
public:
    ExpressionOpOr( TSharedExpression left, TSharedExpression right);

    virtual bool isValid( ) const;

    virtual bool match( TconstSharedLogEntry &entry ) const;

    virtual std::ostream &out( std::ostream &o, bool extended = false ) const;
private:
    TSharedExpression m_left, m_right;
};

typedef boost::shared_ptr<ExpressionOpOr> TSharedExpressionOpOr;
typedef boost::shared_ptr<const ExpressionOpOr> TconstSharedExpressionOpOr;

class ExpressionOpAnd
    :public Expression
{
public:
    ExpressionOpAnd( TSharedExpression left, TSharedExpression right);

    virtual bool isValid( ) const;

    virtual bool match( TconstSharedLogEntry &entry ) const;

    virtual std::ostream &out( std::ostream &o, bool extended = false ) const;
private:
    TSharedExpression m_left, m_right;
};

typedef boost::shared_ptr<ExpressionOpAnd> TSharedExpressionOpAnd;
typedef boost::shared_ptr<const ExpressionOpAnd> TconstSharedExpressionOpAnd;

class ExpressionOpXOr
    :public Expression
{
public:
    ExpressionOpXOr( TSharedExpression left, TSharedExpression right);

    virtual bool isValid( ) const;

    virtual bool match( TconstSharedLogEntry &entry ) const;

    virtual std::ostream &out( std::ostream &o, bool extended = false ) const;
private:
    TSharedExpression m_left, m_right;
};

typedef boost::shared_ptr<ExpressionOpXOr> TSharedExpressionOpXOr;
typedef boost::shared_ptr<const ExpressionOpXOr> TconstSharedExpressionOpXOr;

#endif /* EXPRESSIONOPERATORS_H_ */
