/*
 * LogEntryTableWindow.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogEntryTableWindow.h"

#include "Models/LogEntryTableModel.h"

LogEntryTableWindow::LogEntryTableWindow( boost::shared_ptr<LogEntryTableModel> model )
	:m_model( model )
{
	setModel( model.get() );
    this->horizontalHeader()->moveSection(1, model->columnCount( QModelIndex() )-1 );
    this->verticalHeader()->setDefaultSectionSize( 20 );
    this->setAlternatingRowColors(true);
	show();
}

LogEntryTableWindow::~LogEntryTableWindow()
{

}

void LogEntryTableWindow::updateGeometries()
{
	bool scrollDown = verticalScrollBar()->maximum() == verticalScrollBar()->value();
	QTableView::updateGeometries();
	if( scrollDown )
		scrollToBottom();
}
