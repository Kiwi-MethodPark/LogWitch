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
#include <set>

class QString;

struct ltShPtr
{
  template<class T> bool operator()(const boost::shared_ptr<T> &s1, const boost::shared_ptr<T> &s2) const
  {
    return s1.get() < s2.get();
  }
};

class LogEntryRemoveFilter
	:public LogEntryFilter
{
public:
	LogEntryRemoveFilter( int attrId );
	~LogEntryRemoveFilter();

	void addEntry( boost::shared_ptr<const QString> str );

	void removeEntry( boost::shared_ptr<const QString> str );

	void clear();

	virtual bool filterEntry( TconstSharedLogEntry entry ) const;

private:
	std::set<boost::shared_ptr<const QString>, ltShPtr > m_removeStrings;

	int m_attributeID;
};

#endif /* LOGENTRYREMOVEFILTER_H_ */
