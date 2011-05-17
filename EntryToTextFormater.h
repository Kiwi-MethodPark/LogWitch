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

class EntryToTextFormater {
public:
	EntryToTextFormater();
	virtual ~EntryToTextFormater();

	QString formatEntry( TconstSharedLogEntry entry ) const;
private:
};

#endif /* ENTRYTOTEXTFORMATER_H_ */
