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
#include "LogData/LogEntryAttributeNames.h"

LogEntryTableModel::LogEntryTableModel( boost::shared_ptr<LogEntryParser> parser )
	: m_table( )
	, m_modelConfiguration( parser->getParserModelConfiguration() )
	, m_entryLoader( parser )
	, m_ModelName("Untitled")
    , m_mutex( QMutex::Recursive )
    , m_captureActive( true )
    , m_maxNumberOfEntries( 0 )
    , m_blockInsertingMessages( false )
{
    QObject::connect(dynamic_cast<QObject*>(parser.get()), SIGNAL(newEntry( TconstSharedNewLogEntryMessage )),
                     this, SLOT(insertEntry( TconstSharedNewLogEntryMessage ))
                     , Qt::QueuedConnection );
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

TconstSharedLogEntry LogEntryTableModel::getEntryByRow( const int &row ) const
{
    QMutexLocker lo( &m_mutex );

    if (row < 0 || row >= int(m_table.size() ) )
    {
        qDebug() << "Returning empty item from model: index.row():" << row
        << " table size:" << int( m_table.size() );
        return TconstSharedLogEntry();
    }

    return m_table[row];
}

TconstSharedLogEntry LogEntryTableModel::getEntryByIndex( const QModelIndex &index ) const
{
    QMutexLocker lo( &m_mutex );

    if (index.row() < 0
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
    else if( role == 513 )
    {
        bool rVal = m_modelConfiguration->getFieldShowHint( section );
        return rVal;
    }
    else if( role == 514 )
    {
        return m_modelConfiguration->getFieldOrderHint( section );
    }

    return QVariant();
}

bool LogEntryTableModel::setHeaderData ( int section, Qt::Orientation orientation, const QVariant & value, int role )
{
    if (section >= (m_modelConfiguration->getLogEntryFactory()->getNumberOfFields( ) )
        || section < 0
        || orientation != Qt::Horizontal )
        return false;

    if( role == 512 )
    {
        m_modelConfiguration->setFieldWidthHint( section, value.value<int>(), false );

        // We do not need to emit this, because we change only the default values for the model.
        // emit headerDataChanged( orientation, section, section );
        return true;
    }
    else if( role == 513 )
    {
        m_modelConfiguration->setFieldShowHint( section, value.toBool(), false );
        return true;
    }
    else if( role == 514 )
    {
        m_modelConfiguration->setFieldOrderHint( section, value.toInt(), false );
        return true;
    }

    return false;
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

    if( m_blockInsertingMessages )
    {
        m_blockedInsertedMessages.push_back( mess );
        return;
    }
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
                removeRows( 0, toRemove );
            }
        }
    }

    const int newPos = m_table.size();
    beginInsertRows( QModelIndex(), newPos, newPos + entryCount - 1 );
    m_table.insert( m_table.end(), mess->entries.begin(), mess->entries.end() );
    endInsertRows();
}

void LogEntryTableModel::beginBlockItems()
{
    m_blockInsertingMessages = true;
}

void LogEntryTableModel::endBlockItems()
{
    m_blockInsertingMessages = false;

    while( !m_blockedInsertedMessages.empty() )
    {
        insertEntry( m_blockedInsertedMessages.front() );
        m_blockedInsertedMessages.pop_front();
    }
}

bool LogEntryTableModel::removeRows ( int row, int count, const QModelIndex & parent )
{
    if( row + count > m_table.size() || count == 0 )
        return false;

    beginRemoveRows( parent, row, row + count - 1 );
    m_table.erase( m_table.begin(), m_table.begin() + count );
    endRemoveRows();
    return true;
}

void LogEntryTableModel::setMaxEntries( int max )
{
    m_maxNumberOfEntries = max;
}

void LogEntryTableModel::capture( bool active )
{
    m_captureActive = active;
}

namespace
{
    const QString timestampFormat("yyyy-MM-dd hh:mm:ss.zzz");

    struct ExportFormater
    {
        QString operator()(const QVariant &value)
        {
            QString formatedString;

            switch( value.type() )
            {
            case QVariant::DateTime:
                return value.value<QDateTime>().toString(timestampFormat);
            }

            return value.toString();
        }
    };

}

void LogEntryTableModel::exportToFile( const QString &target )
{
    QMutexLocker lo( &m_mutex );

    QFile file( target );
    file.open(QIODevice::WriteOnly);
    QTextStream str( &file );

    // Determine sort order, we will put the message to the end.
    const int fields = m_modelConfiguration->getLogEntryFactory()->getNumberOfFields();

    // Try to find message field, we will put this to the end of the file!
    LogEntryAttributeNames names;
    int messageID = m_modelConfiguration->getLogEntryFactory()->getNumberOfFields() - 1;
    for( int i = 0; i < m_modelConfiguration->getLogEntryFactory()->getNumberOfFields(); ++i )
    {
        if( m_modelConfiguration->getLogEntryFactory()->getDescShort(i) == "message" )
        {
            messageID = i;
            break;
        }
    }


    std::vector<int> order;
    for ( int i = 0; i < fields; i++ )
    {
        if( i != messageID )
            order.push_back( i );
    }
    order.push_back( messageID );

    QString desc;

    for ( int i = 0; i < int( order.size() ); i++ )
    {
        if( desc.length() )
            desc.append(" - ");

        desc.append( m_modelConfiguration->getLogEntryFactory()->getDescShort( order[i] ) );
        desc += "(" + QString::number(order[i]) +")";

        QString imExExtension( m_modelConfiguration->getLogEntryFactory()->getFieldConfiguration(i).attributeFactory->getImportExportDescription() );
        if( imExExtension.length() > 0 )
        {
            desc.append( ':' );
            desc.append( imExExtension );
        }
    }

    str << "%%LWI_DESC="<< desc << "\n";
    str << "%%LWI_CFGContext="<< m_modelConfiguration->getConfigurationString() << "\n";

    TLogEntryTable::iterator it;
    QRegExp regexLineEnd("(\r\n|\r|\n)");
    QRegExp regexSplitter("( - )");
    for( it = m_table.begin(); it != m_table.end(); ++it)
    {
        QString line;
        for ( int i = 0; i < int( order.size() ); i++ )
        {
            if( line.length() )
                line.append(" - ");

            QString entry( *(*it)->getAttributeAsString(order[i]
                         , ExportToQStringAdapter( m_modelConfiguration->getLogEntryFactory()->getFieldConfiguration(i).attributeFactory ) ) );
            entry.replace(regexLineEnd,"\n " );

            if( i != int( order.size()-1 ) )
              entry.replace(regexSplitter," -- " );

            line.append( entry );
        }
        str << line << "\n";
    }
}
