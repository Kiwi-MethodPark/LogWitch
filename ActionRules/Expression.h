/*
 * Expression.h
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#ifndef EXPRESSION_H_
#define EXPRESSION_H_
#include "LogData/LogEntry.h"

class Expression
{
public:
    virtual ~Expression();

    virtual bool match( TconstSharedLogEntry &entry ) const = 0;
};

typedef boost::shared_ptr<Expression> TSharedExpression;
typedef boost::shared_ptr<const Expression> TconstSharedExpression;


#endif /* EXPRESSION_H_ */
