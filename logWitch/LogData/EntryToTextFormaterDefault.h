/*
 * EntryToTextFormater_Logfile.h
 *
 *  Created on: May 18, 2011
 *      Author: sven
 */

#ifndef ENTRYTOTEXTFORMATERDEFAULT_H_
#define ENTRYTOTEXTFORMATERDEFAULT_H_
#include <LogData/EntryToTextFormater.h>

class EntryToTextFormaterDefault
	: public EntryToTextFormater
{
public:
	virtual QString formatEntry( TconstSharedLogEntry entry ) const;
};

#endif /* ENTRYTOTEXTFORMATER_LOGFILE_H_ */
