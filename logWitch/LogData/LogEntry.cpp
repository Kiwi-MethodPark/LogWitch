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
	: m_attributes( new QVariant[defAttributes.size()] )
	, myFactory( factory )
{
    Q_ASSERT( defAttributes.size() == myFactory->getNumberOfFields() );

    for( int i = 0; i < defAttributes.size(); ++i )
        m_attributes[i] = defAttributes[i];
}

LogEntry::~LogEntry()
{
    delete [] m_attributes;
}

void LogEntry::setAttribute( const QVariant &value, int idx )
{
    Q_ASSERT( idx >= 0 && idx < myFactory->getNumberOfFields() );

    if( value.type() == QVariant::String )
    {
        TSharedQString strIn( new QString( value.toString() ) );
        strIn->squeeze();
        TSharedConstQString str =  myFactory->getCache(idx).getObject( strIn );
        m_attributes[idx] = QVariant::fromValue( str );
    }
    else
    {
        m_attributes[idx] = value;
    }
}

const QVariant &LogEntry::getAttribute( int idx ) const
{
    Q_ASSERT( idx >= 0 && idx < myFactory->getNumberOfFields() );
    return m_attributes[idx];
}

boost::shared_ptr<const QString> LogEntry::getAttributeAsString( int idx ) const
{
    Q_ASSERT( idx >= 0 && idx < myFactory->getNumberOfFields() );

    const QVariant &value = m_attributes[idx];
    if( value.canConvert<TSharedConstQString>() )
        return value.value<TSharedConstQString>();
    else
        return boost::shared_ptr<const QString>( new QString( value.toString() ) );
}

