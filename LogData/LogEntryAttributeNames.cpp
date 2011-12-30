/*
 * LogEntryAttributeNames.cpp
 *
 *  Created on: Jun 23, 2011
 *      Author: sven
 */

#include "LogEntryAttributeNames.h"

LogEntryAttributeNames::LogEntryAttributeNames()
:attDescNumber("number",tr("Number"))
,attDescTimestamp("timestamp",tr("Timestamp"))
,attDescMessage("message",tr("Log Message"))
,attDescLoglevel("level",tr("Loglevel"))
,attDescNDC("ndc",tr("Nested Diagnostic content"))
,attDescThread("thread",tr("Thread"))
,attDescLogger("logger",tr("Logger"))
,attDescFileSource("fsource",tr("File Source"))
,m_defaultCellIfo( false, 150 )
{
    m_defaultCellIfos.insert( StringIntMap::value_type( attDescNumber.first       , EntryConfiguration( false, 60 ) ) );
    m_defaultCellIfos.insert( StringIntMap::value_type( attDescTimestamp.first    , EntryConfiguration( false, 180 ) ) );
    m_defaultCellIfos.insert( StringIntMap::value_type( attDescMessage.first      , EntryConfiguration( false, 500 ) ) );
    m_defaultCellIfos.insert( StringIntMap::value_type( attDescLoglevel.first     , EntryConfiguration( true,  70 ) ) ) ;
    m_defaultCellIfos.insert( StringIntMap::value_type( attDescNDC.first          , EntryConfiguration( true,  100 ) ) );
    m_defaultCellIfos.insert( StringIntMap::value_type( attDescThread.first       , EntryConfiguration( true,  70 ) ) );
    m_defaultCellIfos.insert( StringIntMap::value_type( attDescLogger.first       , EntryConfiguration( true,  250 ) ) );
    m_defaultCellIfos.insert( StringIntMap::value_type( attDescFileSource.first   , EntryConfiguration( true,  150 ) ) );
}

const LogEntryAttributeNames::EntryConfiguration &LogEntryAttributeNames::getDefautlForColumn( const QString &name ) const
{
    StringIntMap::const_iterator it = m_defaultCellIfos.find( name );
    if( it != m_defaultCellIfos.end() )
        return it->second;
    else
        return m_defaultCellIfo;
}

LogEntryAttributeNames::EntryConfiguration::EntryConfiguration( bool caching, int defaultCellWidth )
: caching( caching )
, defaultCellWidth( defaultCellWidth )
{
}
