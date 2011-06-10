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

LogEntryTableWindow::LogEntryTableWindow( boost::shared_ptr<LogEntryTableModel> model, QWidget *parent )
	:QTableView(parent)
	 ,m_model( model )
{
    m_proxyModel = new LogEntryTableFilter(this);
    m_proxyModel->setSourceModel(m_model.get());
    TSharedRuleTable rules( new RuleTable );
    TconstSharedValueGetter left( new ValueGetterConstQString("DEBUG") );
    TconstSharedValueGetterLogEntry right( new ValueGetterLogEntry("Severity", m_model->getParserModelConfiguration() ) );
    if( !right->isValid() )
        qDebug() << "Something is going wrong!!";
    TconstSharedExpression expr( new ExpressionValueGetter( left, right ) );
    TSharedActionDataRewriter action( new ActionDataRewriter( ) );
    action->addChangeSet( Qt::darkMagenta, Qt::ForegroundRole );
    TSharedRule rule( new Rule( expr, action ) );
    rules->addRule( rule );
    m_proxyModel->setRuleTable( (TconstSharedRuleTable)rules );

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
