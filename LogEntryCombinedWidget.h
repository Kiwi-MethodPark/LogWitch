/*
 * LogEntryCombinedWidget.h
 *
 *  Created on: May 17, 2011
 *      Author: sven
 */

#ifndef LOGENTRYCOMBINEDWIDGET_H_
#define LOGENTRYCOMBINEDWIDGET_H_
#include <QtGui>
#include <boost/shared_ptr.hpp>

class LogEntryTableModel;
class LogEntryTableWindow;
class LogEntryFilter;
class QTabWidget;

class LogEntryCombinedWidget
	: public QMdiSubWindow
{
	Q_OBJECT
public:
	LogEntryCombinedWidget( boost::shared_ptr<LogEntryTableModel> model, QWidget *parent = NULL );
	virtual ~LogEntryCombinedWidget();

	void addFilter( boost::shared_ptr<LogEntryFilter> flt );

	QTabWidget *getTabFilterWidget();

	void setDockForFilter( QDockWidget *dock );

private slots:
	void newSelection ( const QItemSelection & selected, const QItemSelection & deselected );

private:
	boost::shared_ptr<LogEntryTableModel> m_model;

	QSplitter *m_splitter;
	QTextEdit *m_text;
	LogEntryTableWindow *m_table;

	QTabWidget *m_myFilterTabs;

	QDockWidget *m_dockFilterShouldDockedTo;
};

#endif /* LOGENTRYCOMBINEDWIDGET_H_ */
