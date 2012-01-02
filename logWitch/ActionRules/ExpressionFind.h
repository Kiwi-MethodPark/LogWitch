/*
 * ExpressionFind.h
 *
 *  Created on: Dec 3, 2011
 *      Author: sven
 */

#ifndef EXPRESSIONFIND_H_
#define EXPRESSIONFIND_H_

#include "Expression.h"
#include "ValueGetter.h"

class ExpressionFind
    : public Expression
{
public:
    ExpressionFind( TconstSharedValueGetter value, const QString &exp);
    ~ExpressionFind();

    bool isValid( ) const;

    bool match( TconstSharedLogEntry &entry ) const;

    std::ostream &out( std::ostream &o, bool extended = false ) const;
private:
    TconstSharedValueGetter m_value;

    QString m_pattern;
};


typedef boost::shared_ptr<ExpressionFind> TSharedExpressionFind;
typedef boost::shared_ptr<const ExpressionFind> TconstSharedExpressionFind;


#endif /* EXPRESSIONFIND_H_ */
