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

class LogEntry {
public:
	LogEntry(const QDateTime &date, boost::shared_ptr<QString> source, boost::shared_ptr<QString> severity, const QString &message );

private:
	QDateTime timestamp;
	boost::shared_ptr<QString> source;
	boost::shared_ptr<QString> severity;
	QString message;
};

#endif /* LOGENTRY_H_ */
