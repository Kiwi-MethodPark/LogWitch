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

class LogEntryParser;
class LogEntryTable;

class LogEntryTableModel: public QAbstractTableModel
{
	Q_OBJECT

public:
	LogEntryTableModel( boost::shared_ptr<LogEntryParser> parser );
	virtual ~LogEntryTableModel();

    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    TconstSharedLogEntry getEntryByIndex( const QModelIndex &index ) const;

public slots:
	void insertEntry( TSharedLogEntry entry );

private:
    boost::shared_ptr<LogEntryTable> m_table;

    QString m_dateTimeConversionString;


    boost::shared_ptr<LogEntryParser> m_entryLoader;
};

#endif /* LOGENTRYTABLEMODEL_H_ */
