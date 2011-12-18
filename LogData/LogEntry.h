/*
 * LogEntry.h
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#ifndef LOGENTRYATTRIBUTES_H_
#define LOGENTRYATTRIBUTES_H_
#include <boost/shared_ptr.hpp>
#include <vector>
#include "Types.h"

class LogEntryFactory;
class QString;

class LogEntry {
public:
	virtual ~LogEntry();

	LogEntry( LogEntryFactory *factory, const std::vector< TSharedConstQString > &defAttributes );

	void setAttribute( TSharedConstQString, int idx );

	TSharedConstQString getAttribute( int idx ) const;

	const LogEntryFactory &getFactory() const { return *myFactory; }

	const TSharedConstQString &operator []( int idx) const;

private:
	std::vector< TSharedConstQString > attributes;

	LogEntryFactory *myFactory;
};

typedef boost::shared_ptr<LogEntry> TSharedLogEntry;
typedef boost::shared_ptr<const LogEntry> TconstSharedLogEntry;

#endif /* LOGENTRYATTRIBUTES_H_ */
