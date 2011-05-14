/*
 * LogEntry.h
 *
 *  Created on: May 13, 2011
 *      Author: sven
 */

#ifndef LOGENTRY_H_
#define LOGENTRY_H_

#include <QDateTime>
#include <QString>

#include <boost/shared_ptr.hpp>

class LogEntryAttributes;

class LogEntry {
public:
	LogEntry(const QDateTime &date, boost::shared_ptr<LogEntryAttributes> attr,  const QString &message );

private:
	QDateTime timestamp;

	boost::shared_ptr<LogEntryAttributes> indexedLogEntries;

	QString message;
};

#endif /* LOGENTRY_H_ */
