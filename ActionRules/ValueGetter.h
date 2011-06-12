/*
 * ValueGetter.h
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#ifndef VALUEGETTER_H_
#define VALUEGETTER_H_
#include <iostream>
#include <boost/shared_ptr.hpp>

#include "LogData/LogEntry.h"
#include "Types.h"

class ValueGetter
{
public:
    virtual ~ValueGetter();

    virtual bool isValid( ) const = 0;

    virtual TSharedConstQString getValue( TconstSharedLogEntry &entry ) const = 0;

    virtual std::ostream &out( std::ostream &o, bool extended = false ) const = 0;
};

typedef boost::shared_ptr<ValueGetter> TSharedValueGetter;
typedef boost::shared_ptr<const ValueGetter> TconstSharedValueGetter;

std::ostream& operator<< (std::ostream &o, const ValueGetter &e);

#endif /* VALUEGETTER_H_ */
