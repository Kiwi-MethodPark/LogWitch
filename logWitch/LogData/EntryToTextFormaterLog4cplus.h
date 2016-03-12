/*
 * EntryToTextFormaterLog4cplus.h
 *
 *  Created on: Jun 2, 2011
 *      Author: sven
 */

#ifndef ENTRYTOTEXTFORMATERLOG4CPLUS_H_
#define ENTRYTOTEXTFORMATERLOG4CPLUS_H_

#include <LogData/EntryToTextFormater.h>

class EntryToTextFormaterLog4cplus
    : public EntryToTextFormater
{
public:
    virtual QString formatEntry( TconstSharedLogEntry entry ) const;
};

#endif /* ENTRYTOTEXTFORMATERLOG4CPLUS_H_ */
