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

class LogEntryCombinedWidget
	: public QSplitter
{
	Q_OBJECT
public:
	LogEntryCombinedWidget( boost::shared_ptr<LogEntryTableModel> model );
	virtual ~LogEntryCombinedWidget();

private slots:
	void newSelection ( const QItemSelection & selected, const QItemSelection & deselected );

private:
	boost::shared_ptr<LogEntryTableModel> m_model;

	QTextEdit *m_text;
	LogEntryTableWindow *m_table;
};

#endif /* LOGENTRYCOMBINEDWIDGET_H_ */
