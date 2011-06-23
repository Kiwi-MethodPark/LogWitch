/*
 * LogEntryTableWindow.h
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#ifndef LOGENTRYTABLEWINDOW_H_
#define LOGENTRYTABLEWINDOW_H_
#include <QtGui>
#include <boost/shared_ptr.hpp>
#include "ActionRules/RuleTable.h"

class LogEntryTableModel;
class QSortFilterProxyModel;
class LogEntryFilter;
class LogEntryTableFilter;

class LogEntryTableWindow
	: public QTableView
{
	Q_OBJECT
public:
	LogEntryTableWindow( boost::shared_ptr<LogEntryTableModel> model, QWidget *parent = NULL );
	virtual ~LogEntryTableWindow();

	QModelIndex mapToSource ( const QModelIndex & proxyIndex ) const;

	void addFilter( boost::shared_ptr<LogEntryFilter> flt );

	void setRuleTable( TconstSharedRuleTable table );

protected:
	void updateGeometries();

private:
	boost::shared_ptr<LogEntryTableModel> m_model;

	QTableView m_tableView;

	LogEntryTableFilter *m_proxyModel;
};

#endif /* LOGENTRYTABLEWINDOW_H_ */
