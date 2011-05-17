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

class LogEntryTableModel;

class LogEntryTableWindow
	: public QTableView
{
	Q_OBJECT
public:
	LogEntryTableWindow( boost::shared_ptr<LogEntryTableModel> model, QWidget *parent = NULL );
	virtual ~LogEntryTableWindow();

protected:
	void updateGeometries();


private:
	boost::shared_ptr<LogEntryTableModel> m_model;

	QTableView m_tableView;


};

#endif /* LOGENTRYTABLEWINDOW_H_ */
