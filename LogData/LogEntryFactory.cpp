/*
 * LogEntryAttributes.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogEntryFactory.h"

#include <QString>
#include <QtCore/QVariant>

#include "Assert.h"
#include "LogData/ObjectCache.hxx"
#include "LogData/LogEntry.h"


LogEntryFactory::LogEntryFactory()
	: m_disallowAddingFields( false )
{

}

LogEntryFactory::~LogEntryFactory()
{

}

boost::shared_ptr<LogEntry> LogEntryFactory::getNewLogEntry()
{
	boost::shared_ptr<LogEntry> attr( new LogEntry(this, m_defaultLine ) );

	return attr;
}

void LogEntryFactory::disallowAddingFields()
{
	m_disallowAddingFields = true;
}

void LogEntryFactory::addField( const QString &descShort, const QString &descLong, bool cacheField )
{
    addField( std::pair<QString,QString>(descShort,descLong), cacheField );
}

void LogEntryFactory::addField( const std::pair<QString,QString> &desc, bool cacheField )
{
	LFA_ASSERT( !disallowAddingFields, "Adding fields is not allowed!" );
	fieldDescriptions.push_back( desc );

	if( cacheField )
	    fieldCaches.push_back( boost::shared_ptr< ObjectCache<ObjectCacheQStringSignaller> >( new ObjectCache<ObjectCacheQStringSignaller> ) );
	else
	    fieldCaches.push_back( boost::shared_ptr< ObjectPasser<ObjectCacheQStringSignaller> >( new ObjectPasser<ObjectCacheQStringSignaller> ) );

	m_defaultLine.push_back( QVariant( fieldCaches.back()->getObject( boost::shared_ptr<QString>(new QString("")) ) ) );
}

int LogEntryFactory::getNumberOfFields( ) const
{
	LFA_ASSERT_D( m_disallowAddingFields, "Getting numbers of fields only allowed if all fields are added!" );
	return fieldDescriptions.size();
}

const QString& LogEntryFactory::getDescLong( int idx ) const
{
	LFA_ASSERT_D( m_disallowAddingFields, "Getting descriptions of fields only allowed if all fields are added!" );
	return fieldDescriptions[idx].second;
}

const QString& LogEntryFactory::getDescShort( int idx ) const
{
	LFA_ASSERT_D( m_disallowAddingFields, "Getting descriptions of fields only allowed if all fields are added!" );
	return fieldDescriptions[idx].first;
}


GetObjectIF<ObjectCacheQStringSignaller>& LogEntryFactory::getCache( int idx )
{
	LFA_ASSERT_D( m_disallowAddingFields, "Getting caches of fields only allowed if all fields are added!" );
	return *(fieldCaches[idx]);
}

const GetObjectIF<ObjectCacheQStringSignaller>& LogEntryFactory::getCache( int idx ) const
{
	LFA_ASSERT_D( m_disallowAddingFields, "Getting caches of fields only allowed if all fields are added!" );
	return *(fieldCaches[idx]);
}
