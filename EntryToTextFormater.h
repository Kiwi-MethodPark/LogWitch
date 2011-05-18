/*
 * EntryToTextFormater.h
 *
 *  Created on: May 17, 2011
 *      Author: sven
 */

#ifndef ENTRYTOTEXTFORMATER_H_
#define ENTRYTOTEXTFORMATER_H_
#include <QtCore/QtCore>
#include "LogData/LogEntry.h"

class EntryToTextFormater
{
public:
	virtual ~EntryToTextFormater(){};

	virtual QString formatEntry( TconstSharedLogEntry entry ) const = 0;
};

#endif /* ENTRYTOTEXTFORMATER_H_ */
