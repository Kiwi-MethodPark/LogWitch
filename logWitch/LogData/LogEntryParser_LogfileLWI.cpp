/*
 * LogEntryParser_LogfileLWI.cpp
 *
 *  Created on: Dec 18, 2011
 *      Author: sven
 */

#include "LogEntryParser_LogfileLWI.h"

#include <QRegExp>
#include <QtCore/QtCore>

#include "LogData/LogEntryAttributeNames.h"
#include "LogData/LogEntryFactory.h"
#include "LogData/LogEntryParserModelConfiguration.h"

LogEntryParser_LogfileLWI::LogEntryParser_LogfileLWI( const QString &filename)
    : m_abort(false )
    , m_logfile( filename )
    , m_logfileStreamReady( false )
    , m_logEntryNumber( 0 )
{
}

LogEntryParser_LogfileLWI::~LogEntryParser_LogfileLWI()
{
    m_abort = true;
    wait();
}

void LogEntryParser_LogfileLWI::startEmiting()
{
    if (!isRunning() && !m_abort )
        start(LowPriority);
}

boost::shared_ptr<LogEntryParserModelConfiguration> LogEntryParser_LogfileLWI::getParserModelConfiguration() const
{
    return m_myModelConfig;
}

bool LogEntryParser_LogfileLWI::initParser()
{
    if (!m_logfile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        m_initError = tr("Unable to open text file!");
        return false;
    }

    m_logfileStream.setDevice( &m_logfile );
    m_logfileStreamReady = true;

    const QString LWIIdString( "%%LWI_DESC=" );

    QString firstLine = m_logfileStream.readLine();
    if ( !firstLine.startsWith( LWIIdString ) )
        return false;

    firstLine.remove( 0, LWIIdString.length() );
    QStringList descriptions = firstLine.split(" - ");

    m_factory.reset( new LogEntryFactory );

    LogEntryAttributeNames names;
    for( QStringList::iterator it = descriptions.begin(); it != descriptions.end(); ++it )
        m_factory->addField( names.getConfiguration( *it ) );

    m_factory->disallowAddingFields();
    m_myModelConfig = boost::shared_ptr<LogEntryParserModelConfiguration>( new LogEntryParserModelConfiguration("LogfileLWI",m_factory) );
    // TODO: Splitstrings?
    //m_myModelConfig->setHierarchySplitString( 4, "\\.");

    for( int i = 0; i < m_factory->getNumberOfFields(); ++i )
    {
        const AttributeConfiguration &cfg = m_factory->getFieldConfiguration( i );
        m_myModelConfig->setFieldWidthHint( i, cfg.defaultCellWidth, true  );
    }

    return true;
}

void LogEntryParser_LogfileLWI::run()
{
    int i = 0;

    TSharedNewLogEntryMessage newEntryMessage( new NewLogEntryMessage );

    forever
    {
        if ( m_abort)
            return;

        TSharedLogEntry entry( getNextLogEntry() );

        if( entry )
            newEntryMessage->entries.push_back( entry );
        else
            m_abort = true;

        i++;
    }

    emit newEntry( newEntryMessage );

    qDebug() << "We got " << i << " entries from logfile.";
}

TSharedLogEntry LogEntryParser_LogfileLWI::logEntryFromString( const QString &str)
{
    TSharedLogEntry entry = m_factory->getNewLogEntry();

    int fieldCount = m_factory->getNumberOfFields();

    return entry;
}

TSharedLogEntry LogEntryParser_LogfileLWI::getNextLogEntry()
{
    TSharedLogEntry entryReturn;

    while( m_logfileStreamReady && !entryReturn )
    {
        QString logEntryString;

        // Step 1
        // Read one entry. One entry is at the end if on the next line is no
        // ' ' space at beginning.
        {
            bool lineComplete = false;
            while( !m_logfileStream.atEnd() && !lineComplete )
            {
                if( m_stashedLine.isNull() )
                    m_stashedLine = m_logfileStream.readLine();

                if( logEntryString.isEmpty() )
                {
                    logEntryString = m_stashedLine;
                    m_stashedLine = QString();
                }
                else
                {
                    if( m_stashedLine.isEmpty() || m_stashedLine.at(0) != ' ' )
                        lineComplete = true;
                    else
                    {
                        logEntryString.append( m_stashedLine );
                        m_stashedLine = QString();
                    }
                }
            }
        }

        // Step 2
        // Analyze and split string to fields.
        {
            QStringList parts = logEntryString.split(" - ");
            if( parts.size() > m_factory->getNumberOfFields() )
            {
                QStringList::iterator it = parts.begin() + m_factory->getNumberOfFields();
                QStringList message;
                for( ; it != parts.end(); ++it )
                    message.push_back( *it );
                parts.erase( it, parts.end() );
                parts.push_back( message.join(" - ") );
            }

            // Check if entry is broken, we will skip it!
            // TODO: Add error message?
            if( parts.size() == m_factory->getNumberOfFields() )
            {
                TSharedLogEntry entry = m_factory->getNewLogEntry();
                for( int i = 0; i < m_factory->getNumberOfFields(); ++i )
                {
                    entry->setAttribute(
                              m_factory->getFieldConfiguration( i ).attributeFactory( parts.at(i) )
                            , i );
                }

                entryReturn = entry;
            }
        }
    }

    return entryReturn;
}
