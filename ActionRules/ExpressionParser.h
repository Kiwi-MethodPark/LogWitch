/*
 * ExpressionParser.h
 *
 *  Created on: Jun 10, 2011
 *      Author: sven
 */

#ifndef EXPRESSIONPARSER_H_
#define EXPRESSIONPARSER_H_
#include <Qt/qstring.h>

class ExpressionParser
{
public:
    ExpressionParser( const QString &expression );
    virtual ~ExpressionParser();
};

#endif /* EXPRESSIONPARSER_H_ */
