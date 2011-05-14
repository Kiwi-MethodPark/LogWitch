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

class LogEntryTable;


class LogEntryTableModel: public QAbstractTableModel
{
	Q_OBJECT

public:
	LogEntryTableModel( boost::shared_ptr<const LogEntryTable> table );
	virtual ~LogEntryTableModel();

    int rowCount(const QModelIndex &parent) const;

    int columnCount(const QModelIndex &parent) const;

    QVariant data(const QModelIndex &index, int role) const;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    Qt::ItemFlags flags(const QModelIndex &index) const;

private:
    boost::shared_ptr<const LogEntryTable> m_table;
};

#endif /* LOGENTRYTABLEMODEL_H_ */
