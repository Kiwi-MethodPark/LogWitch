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

	TSharedRuleTable getRuleTable();

	QTableView *tableView();

public slots:
    /**
     * Switches between the different modes for the quicksearch
     * text line.
     */
    void switchSearchMode();

    /**
     * Updates the view from the quicksearch line
     */
    void updateSearch();

private:
    enum SearchModes { Regex, Text, Expression};
private:
	boost::shared_ptr<LogEntryTableModel> m_model;

	QScrollDownTableView *m_tableView;

	LogEntryTableFilter *m_proxyModel;

	QLineEdit *m_quickSearch;

	SearchModes m_searchMode;

	QPushButton* m_searchModeButton;

	QPushButton* m_markButton;

	TSharedAction m_quickSearchAction;

	TSharedExpression m_quickSearchExp;
};

#endif /* LOGENTRYTABLEWINDOW_H_ */
