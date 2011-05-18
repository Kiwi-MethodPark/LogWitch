/*
 * LogEntryRemoveFilter.h
 *
 *  Created on: May 18, 2011
 *      Author: sven
 */

#ifndef LOGENTRYREMOVEFILTER_H_
#define LOGENTRYREMOVEFILTER_H_
#include "LogEntryFilter.h"
#include <boost/shared_ptr.hpp>
#include <vector>

class QString;

class LogEntryRemoveFilter
	:public LogEntryFilter
{
public:
	LogEntryRemoveFilter();
	~LogEntryRemoveFilter();

	virtual bool filterEntry( TconstSharedLogEntry entry ) const;

private:
	std::vector<boost::shared_ptr<QString> > m_removeStrings;
};

#endif /* LOGENTRYREMOVEFILTER_H_ */
