/*
 * LogEntryTableModel.h
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#ifndef LOGENTRYTABLEMODEL_H_
#define LOGENTRYTABLEMODEL_H_

#include <boost/shared_ptr.hpp>

#include <QAbstractTableModel>
#include <QtCore/QtCore>
#include "LogData/LogEntry.h"
#include "LogEntryParserModelConfiguration.h"
#include "NewLogEntryMessage.h"

class LogEntryParser;
class LogEntryParserModelConfiguration;

class LogEntryTableModel: public QAbstractTableModel
{
	Q_OBJECT

public:
	LogEntryTableModel( boost::shared_ptr<LogEntryParser> parser );
	virtual ~LogEntryTableModel();

	QString getModelName() const { return m_ModelName; };

	void startModel() ;

    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    TconstSharedLogEntry getEntryByIndex( const QModelIndex &index ) const;

    TSharedConstLogEntryParserModelConfiguration getParserModelConfiguration() const;

public slots:
	void insertEntry( TconstSharedNewLogEntryMessage );

	void clearTable();

	/**
	 * This sets the mamimum numbers of entries to the model. If the model
	 * receives more entries it will drop older entries first.
	 *
	 * @param count Numbers of entries to keep at maximum. If set to 0, disables
	 *              this feature.
	 */
	void setMaxEntries( int count );

	void capture( bool active );

    void signalErrorFromParser( QString error );

    void exportToFile( const QString &target );

signals:
    void signalError( QString error );

private:
    typedef std::vector<TconstSharedLogEntry> TLogEntryTable;

    TLogEntryTable m_table;

    TSharedConstLogEntryParserModelConfiguration m_modelConfiguration;

    QString m_dateTimeConversionString;

    boost::shared_ptr<LogEntryParser> m_entryLoader;

    QString m_ModelName;

    mutable QMutex m_mutex;

    bool m_captureActive;

    int m_maxNumberOfEntries;
};

#endif /* LOGENTRYTABLEMODEL_H_ */
