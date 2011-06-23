/*
 * LogEntryAttributes.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogEntryAttributeFactory.h"

#include <QString>

#include "Assert.h"
#include "LogData/ObjectCache.hxx"
#include "LogData/LogEntryAttributes.h"


LogEntryAttributeFactory::LogEntryAttributeFactory()
	: m_disallowAddingFields( false )
{

}

LogEntryAttributeFactory::~LogEntryAttributeFactory() {

}

boost::shared_ptr<LogEntryAttributes> LogEntryAttributeFactory::getNewLogEntryAttributes()
{
	boost::shared_ptr<LogEntryAttributes> attr( new LogEntryAttributes(this, defaultLine ) );

	return attr;
}

void LogEntryAttributeFactory::disallowAddingFields()
{
	m_disallowAddingFields = true;
}

void LogEntryAttributeFactory::addField( const QString &descShort, const QString &descLong, bool cacheField )
{
    addField( std::pair<QString,QString>(descShort,descLong), cacheField );
}

void LogEntryAttributeFactory::addField( const std::pair<QString,QString> &desc, bool cacheField )
{
	LFA_ASSERT( !disallowAddingFields, "Adding fields is not allowed!" );
	fieldDescriptions.push_back( desc );

	if( cacheField )
	    fieldCaches.push_back( boost::shared_ptr< ObjectCache<ObjectCacheQStringSignaller> >( new ObjectCache<ObjectCacheQStringSignaller> ) );
	else
	    fieldCaches.push_back( boost::shared_ptr< ObjectPasser<ObjectCacheQStringSignaller> >( new ObjectPasser<ObjectCacheQStringSignaller> ) );

	defaultLine.push_back( fieldCaches.back()->getObject( boost::shared_ptr<QString>(new QString("")) ) );
}

int LogEntryAttributeFactory::getNumberOfFields( ) const
{
	LFA_ASSERT_D( m_disallowAddingFields, "Getting numbers of fields only allowed if all fields are added!" );
	return fieldDescriptions.size();
}

const QString& LogEntryAttributeFactory::getDescLong( int idx ) const
{
	LFA_ASSERT_D( m_disallowAddingFields, "Getting descriptions of fields only allowed if all fields are added!" );
	return fieldDescriptions[idx].second;
}

const QString& LogEntryAttributeFactory::getDescShort( int idx ) const
{
	LFA_ASSERT_D( m_disallowAddingFields, "Getting descriptions of fields only allowed if all fields are added!" );
	return fieldDescriptions[idx].first;
}


GetObjectIF<ObjectCacheQStringSignaller>& LogEntryAttributeFactory::getCache( int idx )
{
	LFA_ASSERT_D( m_disallowAddingFields, "Getting caches of fields only allowed if all fields are added!" );
	return *(fieldCaches[idx]);
}

const GetObjectIF<ObjectCacheQStringSignaller>& LogEntryAttributeFactory::getCache( int idx ) const
{
	LFA_ASSERT_D( m_disallowAddingFields, "Getting caches of fields only allowed if all fields are added!" );
	return *(fieldCaches[idx]);
}
