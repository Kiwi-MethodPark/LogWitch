/*
 * LogEntryTableModel.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogEntryTableModel.h"

#include <iostream>

#include <QtCore/QtCore>
#include <QtCore/QVariant>
#include <QDateTime>
 #include <QMutexLocker>

#include "LogData/LogEntry.h"
#include "LogData/LogEntryFactory.h"
#include "LogData/LogEntryParser.h"
#include "LogData/LogEntryParserModelConfiguration.h"

LogEntryTableModel::LogEntryTableModel( boost::shared_ptr<LogEntryParser> parser )
	: m_table( )
	, m_modelConfiguration( parser->getParserModelConfiguration() )
	, m_dateTimeConversionString("dd.MM.yyyy hh:mm:ss.zzz")
	, m_entryLoader( parser )
	, m_ModelName("Untitled")
    , m_mutex( QMutex::Recursive )
    , m_captureActive( true )
    , m_maxNumberOfEntries( 0 )
{
    QObject::connect(dynamic_cast<QObject*>(parser.get()), SIGNAL(newEntry( TconstSharedNewLogEntryMessage )),
                     this, SLOT(insertEntry( TconstSharedNewLogEntryMessage )) );
    QObject::connect(dynamic_cast<QObject*>(parser.get()), SIGNAL(signalError( QString )),
                     this, SLOT(signalErrorFromParser( QString )) );

    m_ModelName = parser->getName();
}

LogEntryTableModel::~LogEntryTableModel()
{

}

void LogEntryTableModel::startModel()
{
	m_entryLoader->startEmiting();
}

void LogEntryTableModel::signalErrorFromParser( QString error )
{
    qDebug() << "error from parser received: " << error;
    emit signalError( error );
}

TSharedConstLogEntryParserModelConfiguration LogEntryTableModel::getParserModelConfiguration() const
{
	return m_modelConfiguration;
}

int LogEntryTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    QMutexLocker lo( &m_mutex );

    return m_table.size();
}

int LogEntryTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    QMutexLocker lo( &m_mutex );

    int value = m_modelConfiguration->getLogEntryFactory()->getNumberOfFields( );
    return value;
}

TconstSharedLogEntry LogEntryTableModel::getEntryByIndex( const QModelIndex &index ) const
{
    QMutexLocker lo( &m_mutex );

    if (index.column() >= (m_modelConfiguration->getLogEntryFactory()->getNumberOfFields( ) )
      || index.column() < 0
      || index.row() < 0
      || index.row() >= int(m_table.size() ) )
    {
      qDebug() << "Returning empty item from model: index.column():" << index.column()
              << " index.row():" << index.row()
              << " table size:" << int(m_table.size() )
              << " model columns:" << (m_modelConfiguration->getLogEntryFactory()->getNumberOfFields( ) );
      return TconstSharedLogEntry();
    }

    return m_table[index.row()];
}

QVariant LogEntryTableModel::data(const QModelIndex &index, int role) const
{
    QMutexLocker lo( &m_mutex );

    if (!index.isValid())
        return QVariant();

    if (index.column() >= (m_modelConfiguration->getLogEntryFactory()->getNumberOfFields( ) )
    		|| index.column() < 0
    		|| index.row() < 0
    		|| index.row() >= int(m_table.size() ) )
        return QVariant();

    if (role == Qt::DisplayRole)
    {
    	TconstSharedLogEntry entry = m_table[index.row()];
    	const QVariant &var = entry->getAttribute( index.column() );
    	if( var.canConvert<TSharedConstQString>() )
    	{
    	    // Return native QString instead our special pointer.
    	    QString rv = *var.value<TSharedConstQString>();
    	    return QVariant(rv );
    	}

    	return var;
    }
    return QVariant();
}

QVariant LogEntryTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section >= (m_modelConfiguration->getLogEntryFactory()->getNumberOfFields( ) ) || section < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
		if (orientation == Qt::Horizontal)
		{
			return m_modelConfiguration->getLogEntryFactory()->getDescLong( section );
		}
    }
    else if( role == 512 )
    {
    	int rVal = 150;

  		rVal = m_modelConfiguration->getFieldWidthHint( section );

    	return rVal;
    }

    return QVariant();
}

void LogEntryTableModel::clearTable()
{
    QMutexLocker lo( &m_mutex );
    beginResetModel();
    m_table.clear();
    endResetModel();
}

void LogEntryTableModel::insertEntry( TconstSharedNewLogEntryMessage mess )
{
    if( !m_captureActive || mess->entries.empty() )
        return;

    QMutexLocker lo( &m_mutex );

    size_t entryCount = mess->entries.size();

    if( m_maxNumberOfEntries > 0 )
    {
        // Remove elements to match maxEntriesFirst.
        const int toRemove = m_table.size() + entryCount - m_maxNumberOfEntries;
        if( toRemove > 0 )
        {
            if( size_t( toRemove ) >= m_table.size() )
            {
                // We will clear the table and add as much elements as we can add to our model
                beginResetModel();
                const int frontEntriesDropped = entryCount - m_maxNumberOfEntries;
                Q_ASSERT( frontEntriesDropped >= 0 );
                m_table.clear();

                std::list<TSharedLogEntry>::const_iterator it;
                int i = 0;
                for( it = mess->entries.begin(); i < frontEntriesDropped; ++i, ++it )
                    ;

                m_table.insert( m_table.end()
                        , it
                        , mess->entries.end() );
                endResetModel();

                return;
            }
            else
            {
                beginRemoveRows( QModelIndex(), 0, toRemove - 1 );
                m_table.erase( m_table.begin(), m_table.begin() + toRemove );
                endRemoveRows();
            }
        }
    }

    const int newPos = m_table.size();
    beginInsertRows( QModelIndex(), newPos, newPos + entryCount - 1 );
    m_table.insert( m_table.end(), mess->entries.begin(), mess->entries.end() );
    endInsertRows();
}

void LogEntryTableModel::setMaxEntries( int max )
{
    m_maxNumberOfEntries = max;
}

void LogEntryTableModel::capture( bool active )
{
    m_captureActive = active;
}

void LogEntryTableModel::exportToFile( const QString &target )
{
    QFile file( target );
    file.open(QIODevice::WriteOnly);
    QTextStream str( &file );

    // Determine sort order, we will put the message to the end.
    const int fields = m_modelConfiguration->getLogEntryFactory()->getNumberOfFields();
    std::vector<int> order;
    for ( int i = 0; i < fields; i++ )
    {
        if( i != 2 )
            order.push_back( i );
    }
    order.push_back( 2 );

    QString desc;

    for ( int i = 0; i < int( order.size() ); i++ )
    {
        if( desc.length() )
            desc.append(" - ");

        desc.append( m_modelConfiguration->getLogEntryFactory()->getDescShort( order[i] ) );
    }

    str << "%%LFA_DESC="<< desc << "\n";

    TLogEntryTable::iterator it;
    QRegExp regexLineEnd("(\r\n|\r|\n)");
    for( it = m_table.begin(); it != m_table.end(); ++it)
    {
        QString line;
        for ( int i = 0; i < int( order.size() ); i++ )
        {
            if( line.length() )
                line.append(" - ");

            QString entry( *(*it)->getAttributeAsString(order[i]) );
            entry.replace(regexLineEnd,"\n " );

            line.append( entry );
        }
        str << line << "\n";
    }
}
