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
class QScrollDownTableView;

class LogEntryTableWindow
	: public QWidget
{
	Q_OBJECT
public:
	LogEntryTableWindow( boost::shared_ptr<LogEntryTableModel> model, QWidget *parent = NULL );
	virtual ~LogEntryTableWindow();

	QModelIndex mapToSource ( const QModelIndex & proxyIndex ) const;

	void addFilter( boost::shared_ptr<LogEntryFilter> flt );

	void setRuleTable( TconstSharedRuleTable table );

	QTableView *tableView();

private:
	boost::shared_ptr<LogEntryTableModel> m_model;

	QScrollDownTableView *m_tableView;

	LogEntryTableFilter *m_proxyModel;

	QLineEdit *m_quickSearch;
};

#endif /* LOGENTRYTABLEWINDOW_H_ */
