/*
 * FieldDescriptor.cpp
 *
 *  Created on: Jun 5, 2011
 *      Author: sven
 */

#include "ActionRules/ValueGetterLogEntry.h"

#include <boost/shared_ptr.hpp>

#include "LogData/LogEntryAttributeFactory.h"
#include "LogData/LogEntryAttributes.h"
#include "LogData/LogEntryParserModelConfiguration.h"

ValueGetterLogEntry::ValueGetterLogEntry(const QString &name, boost::shared_ptr<const LogEntryParserModelConfiguration> configuration )
    :m_name( name )
    , m_configuration( configuration )
    , m_fieldId( -1 )
{
    int fieldCount = configuration->getLogEntryAttributeFactory()->getNumberOfFields();
    for( int i = 0; i < fieldCount; i++ )
    {
        if( name == configuration->getLogEntryAttributeFactory()->getDescription( i ) )
        {
            m_fieldId = i;
            break;
        }
    }
}

ValueGetterLogEntry::~ValueGetterLogEntry()
{
}

TSharedConstQString ValueGetterLogEntry::getValue( TconstSharedLogEntry &entry ) const
{
    return entry->getAttributes().getAttribute( m_fieldId );
}

bool ValueGetterLogEntry::isValid( ) const
{
    return m_fieldId >= 0;
}

int ValueGetterLogEntry::getID( ) const
{
    return m_fieldId;
}

std::ostream &ValueGetterLogEntry::out( std::ostream &o, bool extended ) const
{
    if( extended )
    {
        o << "ValueGetterLogEntry{"<< m_name.toStdString() << "}";
        return o;
    }
    else
    {
        o << m_name.toStdString();
        return o;
    }
}
