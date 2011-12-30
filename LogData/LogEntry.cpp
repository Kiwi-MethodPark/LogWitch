/*
 * LogEntryAttributes.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogData/LogEntry.h"

#include <QString>
#include <QtCore/QVariant>

#include "LogData/ObjectCache.hxx"
#include "LogData/LogEntryFactory.h"
#include <QtCore/QtCore>

LogEntry::LogEntry( LogEntryFactory *factory, const std::vector< QVariant > &defAttributes )
	: m_attributes( defAttributes )
    , m_attributesStringCache( defAttributes.size() )
	, myFactory( factory )
{
    updateStringRepresentation();
}

LogEntry::~LogEntry()
{
}

void LogEntry::setAttribute( const QVariant &value, int idx )
{
    if( value.canConvert<TSharedConstQString>() )
    {
        TSharedConstQString str = value.value<TSharedConstQString>();
        str = myFactory->getCache(idx).getObject( str );
        m_attributes[idx] = QVariant::fromValue( str );
        m_attributesStringCache[idx] = str;
    }
    else if( value.type() == QVariant::String )
    {
        TSharedQString strIn( new QString( value.toString() ) );
        TSharedConstQString str =  myFactory->getCache(idx).getObject( strIn );
        m_attributes[idx] = QVariant::fromValue( str );
        m_attributesStringCache[idx] = str;
    }
    else
    {
        TSharedQString str( new QString );
        (*str) = value.toString();

        m_attributes[idx] = value;
        m_attributesStringCache[idx] = str;
    }
}

void LogEntry::updateStringRepresentation()
{
    for( unsigned int idx = 0; idx < m_attributes.size(); ++idx  )
    {
        QVariant value = m_attributes[idx];
        setAttribute( value, idx );
    }
}

const QVariant &LogEntry::getAttribute( int idx ) const
{
    return m_attributes[idx];
}


boost::shared_ptr<const QString> LogEntry::getAttributeAsString( int idx ) const
{
	return m_attributesStringCache[idx];
}

