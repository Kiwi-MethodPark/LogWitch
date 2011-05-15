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
public:
	LogEntryTableWindow( boost::shared_ptr<LogEntryTableModel> model );
	virtual ~LogEntryTableWindow();

	boost::shared_ptr<LogEntryTableModel> m_model;

protected:
	void updateGeometries();

private:
	QTableView m_tableView;


};

#endif /* LOGENTRYTABLEWINDOW_H_ */
