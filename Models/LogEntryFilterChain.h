/*
 * LogEntryFilterChain.h
 *
 *  Created on: May 19, 2011
 *      Author: sven
 */

#ifndef LOGENTRYFILTERCHAIN_H_
#define LOGENTRYFILTERCHAIN_H_

#include <boost/shared_ptr.hpp>
#include "Models/LogEntryFilter.h"
#include <list>

class LogEntryFilterChain
	: public LogEntryFilter
{
public:
	LogEntryFilterChain();

	virtual ~LogEntryFilterChain();

	void addFilter( boost::shared_ptr<const LogEntryFilter> );

	void removeFilter( boost::shared_ptr<const LogEntryFilter> );

	virtual bool filterEntry( TconstSharedLogEntry entry ) const;

private:
	typedef std::list< boost::shared_ptr< const LogEntryFilter> > TFilterChain;
	TFilterChain m_filterChain;
};

#endif /* LOGENTRYFILTERCHAIN_H_ */
