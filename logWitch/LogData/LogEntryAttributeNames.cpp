/*
 * LogEntryAttributeNames.cpp
 *
 *  Created on: Jun 23, 2011
 *      Author: sven
 */

#include "LogData/LogEntryAttributeNames.h"

namespace
{
    struct QStringToNumber
    {
        QVariant operator()(const QString &str )
        {
            bool ok = false;
            int value = str.toInt(&ok);
            if( !ok )
                return QVariant( int(0) );
            else
                return QVariant( value );
        }
    };

    struct QStringToDateTime
    {
        QStringToDateTime( const QString &fmt ): timeFormat( fmt ) {}

        QVariant operator()(const QString &str )
        {
            QDateTime timestamp = QDateTime::fromString( str, timeFormat );
            return timestamp;
        }

        QString timeFormat;
    };

    struct QStringToVariant
    {
        QVariant operator()(const QString &str )
        {
            return str;
        }
    };
}

LogEntryAttributeNames::LogEntryAttributeNames()
:attDescNumber("number",tr("Number"))
,attDescTimestamp("timestamp",tr("Timestamp"))
,attDescMessage("message",tr("Log Message"))
,attDescLoglevel("level",tr("Loglevel"))
,attDescNDC("ndc",tr("Nested Diagnostic content"))
,attDescThread("thread",tr("Thread"))
,attDescLogger("logger",tr("Logger"))
,attDescFileSource("fsource",tr("File Source"))
,m_defaultCellIfo( false, 150, AttributeConfiguration::TQStringPair("unknown", tr("Unknown") ), QStringToVariant() )
{
    m_defaultCellIfos.insert( StringIntMap::value_type( attDescNumber.first       , AttributeConfiguration( false, 60 , attDescNumber
            , QStringToNumber() ) ) );
    m_defaultCellIfos.insert( StringIntMap::value_type( attDescTimestamp.first    , AttributeConfiguration( false, 180, attDescTimestamp
            , QStringToDateTime("yyyy-MM-dd HH:mm:ss,zzz") ) ) );
    m_defaultCellIfos.insert( StringIntMap::value_type( attDescMessage.first      , AttributeConfiguration( false, 500, attDescMessage
            , QStringToVariant() ) ) );
    m_defaultCellIfos.insert( StringIntMap::value_type( attDescLoglevel.first     , AttributeConfiguration( true,  70 , attDescLoglevel
            , QStringToVariant() ) ) );
    m_defaultCellIfos.insert( StringIntMap::value_type( attDescNDC.first          , AttributeConfiguration( true,  100, attDescNDC
            , QStringToVariant() ) ) );
    m_defaultCellIfos.insert( StringIntMap::value_type( attDescThread.first       , AttributeConfiguration( true,  70 , attDescThread
            , QStringToVariant() ) ) );
    m_defaultCellIfos.insert( StringIntMap::value_type( attDescLogger.first       , AttributeConfiguration( true,  250, attDescLogger
            , QStringToVariant() ) ) );
    m_defaultCellIfos.insert( StringIntMap::value_type( attDescFileSource.first   , AttributeConfiguration( true,  150, attDescFileSource
            , QStringToVariant() ) ) );
}

const AttributeConfiguration &LogEntryAttributeNames::getConfiguration( const QString &name ) const
{
    StringIntMap::const_iterator it = m_defaultCellIfos.find( name );
    if( it != m_defaultCellIfos.end() )
        return it->second;
    else
        return m_defaultCellIfo;
}

AttributeConfiguration::AttributeConfiguration( bool caching, int defaultCellWidth, TQStringPair namesIn,  EntryFactoryFunction factory )
: caching( caching )
, defaultCellWidth( defaultCellWidth )
, names( namesIn )
, attributeFactory( factory )
{
}
