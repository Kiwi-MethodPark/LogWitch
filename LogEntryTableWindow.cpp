/*
 * LogEntryTableWindow.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogEntryTableWindow.h"

#include "Models/LogEntryTableModel.h"
#include "Models/LogEntryTableFilter.h"

LogEntryTableWindow::LogEntryTableWindow( boost::shared_ptr<LogEntryTableModel> model, QWidget *parent )
	:QTableView(parent)
	 ,m_model( model )
{
    m_proxyModel = new LogEntryTableFilter(this);
    m_proxyModel->setSourceModel(m_model.get());
	setModel( m_proxyModel );
    this->horizontalHeader()->moveSection(1, model->columnCount( QModelIndex() )-1 );
    this->horizontalHeader()->setMovable( true );
    this->verticalHeader()->setDefaultSectionSize( 20 );
    this->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    this->verticalHeader()->hide();
    this->setAlternatingRowColors(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode( QAbstractItemView::SingleSelection );

	// Qt::Horizontal
	int count = m_model->columnCount( QModelIndex() );
	for( int col = 0; col < count; col ++)
	{
		int width = m_proxyModel->headerData( col, Qt::Horizontal, 512 ).value<int>();
		this->horizontalHeader()->resizeSection( col, width );
	}
}

void LogEntryTableWindow::addFilter( boost::shared_ptr<LogEntryFilter> flt )
{
	m_proxyModel->addFilter( flt );
}

QModelIndex LogEntryTableWindow::mapToSource ( const QModelIndex & proxyIndex ) const
{
	return m_proxyModel->mapToSource ( proxyIndex );
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
