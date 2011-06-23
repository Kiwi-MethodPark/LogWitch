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
#include <QMetaType>

#include <boost/shared_ptr.hpp>

class LogEntryAttributes;

class LogEntry {
public:
	LogEntry( boost::shared_ptr<LogEntryAttributes> attr );

	const LogEntryAttributes &getAttributes() const;

	LogEntryAttributes &getAttributes();

private:
	boost::shared_ptr<LogEntryAttributes> indexedLogEntries;

};

typedef boost::shared_ptr<LogEntry> TSharedLogEntry;
typedef boost::shared_ptr<const LogEntry> TconstSharedLogEntry;

Q_DECLARE_METATYPE ( TSharedLogEntry );

#endif /* LOGENTRY_H_ */
