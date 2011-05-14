/*
 * LogEntryAttributes.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogEntryAttributeFactory.h"

#include <QString>

#include "Assert.h"
#include "LogData/StringCache.h"
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

void LogEntryAttributeFactory::addField( const QString &description )
{
	LFA_ASSERT( !disallowAddingFields, "Adding fields is not allowed!" );
	fieldDescriptions.push_back( description );
	fieldCaches.resize( fieldDescriptions.size() );
	defaultLine.push_back( fieldCaches.back().getString( boost::shared_ptr<QString>(new QString("")) ) );
}

int LogEntryAttributeFactory::getNumberOfFields( ) const
{
	LFA_ASSERT_D( m_disallowAddingFields, "Getting numbers of fields only allowed if all fields are added!" );
	return fieldDescriptions.size();
}

const QString& LogEntryAttributeFactory::getDescription( int idx ) const
{
	LFA_ASSERT_D( m_disallowAddingFields, "Getting descriptions of fields only allowed if all fields are added!" );
	return fieldDescriptions[idx];
}

StringCache& LogEntryAttributeFactory::getCache( int idx )
{
	LFA_ASSERT_D( m_disallowAddingFields, "Getting caches of fields only allowed if all fields are added!" );
	return fieldCaches[idx];
}
