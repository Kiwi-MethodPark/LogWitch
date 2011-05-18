/*
 * LogEntryTableFilter.h
 *
 *  Created on: May 18, 2011
 *      Author: sven
 */

#ifndef LOGENTRYTABLEFILTER_H_
#define LOGENTRYTABLEFILTER_H_

#include <QSortFilterProxyModel>

class LogEntryTableModel;

class LogEntryTableFilter
	: public QSortFilterProxyModel
{
	Q_OBJECT
public:
	LogEntryTableFilter( QObject *parent );

	void setSourceModel( QAbstractItemModel *model );

	virtual ~LogEntryTableFilter();

protected:
	bool filterAcceptsRow ( int source_row, const QModelIndex & source_parent ) const;

private:
	LogEntryTableModel *m_model;
};

#endif /* LOGENTRYTABLEFILTER_H_ */
