/*
 * LogEntryTableWindow.cpp
 *
 *  Created on: May 14, 2011
 *      Author: sven
 */

#include "LogEntryTableWindow.h"

#include "Models/LogEntryTableModel.h"
#include "Models/LogEntryTableFilter.h"
#include "ActionRules/ValueGetterConstQString.h"
#include "ActionRules/RuleTable.h"
#include "ActionRules/ActionDataRewriter.h"
#include "ActionRules/ExpressionValueGetter.h"
#include "ActionRules/ValueGetterLogEntry.h"
#include "GUITools/QScrollDownTableView.h"

LogEntryTableWindow::LogEntryTableWindow( boost::shared_ptr<LogEntryTableModel> model, QWidget *parent )
	:QWidget(parent)
	 ,m_model( model )
     ,m_tableView( new QScrollDownTableView(  ) )
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    this->setLayout(layout);

    m_proxyModel = new LogEntryTableFilter(m_tableView);
    m_proxyModel->setSourceModel(m_model.get());

    m_tableView->setModel( m_proxyModel );
    m_tableView->horizontalHeader()->moveSection(1, model->columnCount( QModelIndex() )-1 );
    m_tableView->horizontalHeader()->setMovable( true );
    m_tableView->verticalHeader()->setDefaultSectionSize( 20 );
    m_tableView->verticalHeader()->setResizeMode(QHeaderView::Fixed);
    m_tableView->verticalHeader()->hide();
    m_tableView->setAlternatingRowColors(true);
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView->setSelectionMode( QAbstractItemView::SingleSelection );

	// Qt::Horizontal
	int count = m_model->columnCount( QModelIndex() );
	for( int col = 0; col < count; col ++)
	{
		int width = m_proxyModel->headerData( col, Qt::Horizontal, 512 ).value<int>();
		m_tableView->horizontalHeader()->resizeSection( col, width );
	}

	m_quickSearch = new QLineEdit;
	//m_quickSearch->setMaximumSize(QWIDGETSIZE_MAX,20);
	layout->addWidget( m_quickSearch );
	layout->addWidget(m_tableView);
}

TSharedRuleTable LogEntryTableWindow::getRuleTable()
{
    return m_proxyModel->getRuleTable();
}

void LogEntryTableWindow::addFilter( boost::shared_ptr<LogEntryFilter> flt )
{
	m_proxyModel->addFilter( flt );
}

QModelIndex LogEntryTableWindow::mapToSource ( const QModelIndex & proxyIndex ) const
{
	return m_proxyModel->mapToSource ( proxyIndex );
}

QTableView *LogEntryTableWindow::tableView()
{
    return m_tableView;
}

LogEntryTableWindow::~LogEntryTableWindow()
{

}


