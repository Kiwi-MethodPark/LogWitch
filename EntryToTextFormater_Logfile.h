/*
 * EntryToTextFormater_Logfile.h
 *
 *  Created on: May 18, 2011
 *      Author: sven
 */

#ifndef ENTRYTOTEXTFORMATER_LOGFILE_H_
#define ENTRYTOTEXTFORMATER_LOGFILE_H_
#include "EntryToTextFormater.h"

class EntryToTextFormater_Logfile
	: public EntryToTextFormater
{
public:
	virtual QString formatEntry( TconstSharedLogEntry entry ) const;
};

#endif /* ENTRYTOTEXTFORMATER_LOGFILE_H_ */
