/*
 * LogEntryCombinedWidget.cpp
 *
 *  Created on: May 17, 2011
 *      Author: sven
 */

#include "LogEntryCombinedWidget.h"
#include "LogEntryTableWindow.h"
#include <QtGui>
#include <QtCore/QtCore>
#include "Models/LogEntryTableModel.h"
#include "EntryToTextFormater.h"
#include "LogData/LogEntryParserModelConfiguration.h"
#include "LogData/LogEntryAttributeFactory.h"
#include "Models/StringCacheTreeModel.h"

LogEntryCombinedWidget::LogEntryCombinedWidget( boost::shared_ptr<LogEntryTableModel> model, QWidget *parent )
	: QMdiSubWindow( parent )
	, m_model( model )
	, m_splitter( new QSplitter(Qt::Vertical, this) )
	, m_myFilterTabs( NULL )
	, m_dockFilterShouldDockedTo( NULL )
{
	m_table = new LogEntryTableWindow( model, this );
	m_text = new QTextEdit("<b>Log Message viewer</b>",this);

    QObject::connect(m_table->selectionModel(), SIGNAL(selectionChanged ( const QItemSelection & , const QItemSelection & )),
                     SLOT(newSelection ( const QItemSelection &, const QItemSelection & )));

    QObject::connect(this, SIGNAL(aboutToActivate()),
                     this, SLOT(getFocused() ) );

    m_splitter->addWidget( m_table );
    m_splitter->addWidget( m_text );

    m_splitter->setStretchFactor( 0, 20 );
    this->setWidget( m_splitter );
    resize( 800, 500 );

    // Just generate the tab here, because we want to catch all filter entries.
    getTabFilterWidget();
}

void LogEntryCombinedWidget::setDockForFilter( QDockWidget *dock )
{
	m_dockFilterShouldDockedTo = dock;
	if( isActiveWindow () )
		getFocused();
}

void LogEntryCombinedWidget::getFocused()
{
	if( m_dockFilterShouldDockedTo != NULL )
	{
		m_dockFilterShouldDockedTo->setWidget( getTabFilterWidget() );
	}
}

QTabWidget *LogEntryCombinedWidget::getTabFilterWidget()
{
	if ( m_myFilterTabs == NULL )
	{
		QTabWidget *tabs = new QTabWidget( );

		int attributes = m_model->getParserModelConfiguration()->getLogEntryAttributeFactory()->getNumberOfFields();
		for(int attr = 0; attr < attributes; attr++ )
		{
			QTreeView *view = new QTreeView;
			StringCacheTreeModel *strModel = new StringCacheTreeModel(view
					, &m_model->getParserModelConfiguration()->getLogEntryAttributeFactory()->getCache(attr)
					, attr
					, m_model->getParserModelConfiguration()->getHierarchySplitString(attr) );

			if( strModel->getFilter() )
				addFilter( strModel->getFilter() );

			view->setModel(strModel);
			tabs->addTab( view, m_model->getParserModelConfiguration()->getLogEntryAttributeFactory()->getDescription(attr));
			view->show();
		}
		m_myFilterTabs = tabs;
	}

	return m_myFilterTabs;
}

void LogEntryCombinedWidget::addFilter( boost::shared_ptr<LogEntryFilter> flt )
{
	m_table->addFilter( flt);
}

void LogEntryCombinedWidget::newSelection ( const QItemSelection & selected, const QItemSelection & )
{
	qDebug() << "New selection of size: " << selected.size();

	TconstSharedLogEntry entry = m_model->getEntryByIndex( m_table->mapToSource( selected.front().topLeft() ) );
	m_text->setHtml( m_model->getParserModelConfiguration()->getEntryToTextFormater()->formatEntry( entry ) );
}

LogEntryCombinedWidget::~LogEntryCombinedWidget()
{
	if( m_myFilterTabs && m_dockFilterShouldDockedTo->widget() == m_myFilterTabs )
		m_dockFilterShouldDockedTo->setWidget( NULL );
}
