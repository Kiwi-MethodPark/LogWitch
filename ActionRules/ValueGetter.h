/*
 * ValueGetter.h
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#ifndef VALUEGETTER_H_
#define VALUEGETTER_H_
#include <boost/shared_ptr.hpp>

#include "LogData/LogEntry.h"
#include "Types.h"

class ValueGetter
{
public:
    virtual ~ValueGetter();

    virtual TSharedConstQString getValue( TconstSharedLogEntry &entry ) const = 0;
};

typedef boost::shared_ptr<ValueGetter> TSharedValueGetter;
typedef boost::shared_ptr<const ValueGetter> TconstSharedValueGetter;


#endif /* VALUEGETTER_H_ */
