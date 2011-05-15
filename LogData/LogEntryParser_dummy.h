/*
 * LogEntryParser_dummy.h
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#ifndef LOGENTRYPARSER_DUMMY_H_
#define LOGENTRYPARSER_DUMMY_H_

#include <QtCore/QtCore>

#include "LogData/LogEntryFactory.h"
#include "LogData/LogEntryParser.h"

/**
 * This class is a dummy for testing without a valid logfile.
 * Generates dummy entries.
 */
class LogEntryParser_dummy
	: public QThread
	, public LogEntryParser
{
	Q_OBJECT
public:
	LogEntryParser_dummy();
	virtual ~LogEntryParser_dummy();

	void startEmiting();

	void addEntries( int count );

	virtual boost::shared_ptr<const LogEntryAttributeFactory> getLogEntryAttributeFactory() const;

signals:
	void newEntry( TSharedLogEntry );

protected:
    void run();

private:
	TSharedLogEntry getNextLogEntry();

	LogEntryFactory myFactory;

	int m_entries;

	bool m_abort;

	int m_count;
};

#endif /* LOGENTRYPARSER_DUMMY_H_ */
