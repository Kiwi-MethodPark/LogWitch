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
#include "FilterListView.h"

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

    QObject::connect(dynamic_cast<QObject*>(m_model.get()), SIGNAL(signalError( QString )),
                     this, SLOT(errorFromModel( QString )) );

    m_splitter->addWidget( m_table );
    m_splitter->addWidget( m_text );

    m_splitter->setStretchFactor( 0, 20 );
    this->setWidget( m_splitter );
    resize( 800, 500 );

    // Just generate the tab here, because we want to catch all filter entries.
    getTabFilterWidget();
    QMdiSubWindow::setAttribute(Qt::WA_DeleteOnClose,true);
}

void LogEntryCombinedWidget::errorFromModel( QString error )
{
    QMessageBox msgBox;
    QString errorText;
    errorText+= "Erorr received: " + error;
    msgBox.setText( errorText );
    msgBox.setInformativeText("Close window now?");
    msgBox.setStandardButtons(QMessageBox::Ignore | QMessageBox::Close );
    msgBox.setDefaultButton(QMessageBox::Ignore);

    int ret =  msgBox.exec();
    if( ret == QMessageBox::Close )
    {
        this->deleteLater();
    }
}

void LogEntryCombinedWidget::setDockForFilter( QDockWidget *dock )
{
    dock->setWidget( getTabFilterWidget( ) );
}

void LogEntryCombinedWidget::clearTable( )
{
    m_model->clearTable();
}

QTabWidget *LogEntryCombinedWidget::getTabFilterWidget()
{
	if ( m_myFilterTabs == NULL )
	{
		QTabWidget *tabs = new QTabWidget( );

		int attributes = m_model->getParserModelConfiguration()->getLogEntryAttributeFactory()->getNumberOfFields();
		for(int attr = 0; attr < attributes; attr++ )
		{
		    FilterListView *view = new FilterListView( this, m_model->getParserModelConfiguration(), attr );
		    view->addToTabs( tabs, this );
		}
		m_myFilterTabs = tabs;
	}

	return m_myFilterTabs;
}

TSharedCompiledRulesStateSaver LogEntryCombinedWidget::getCompiledRules()
{
    return TSharedCompiledRulesStateSaver( new CompiledRulesStateSaver( m_model->getParserModelConfiguration() ) );
    //return TSharedCompiledRulesStateSaver( new CompiledRulesStateSaver( TSharedConstLogEntryParserModelConfiguration() ) );
}

void LogEntryCombinedWidget::addFilter( boost::shared_ptr<LogEntryFilter> flt )
{
	m_table->addFilter( flt);
}

void LogEntryCombinedWidget::newSelection ( const QItemSelection & selected, const QItemSelection & )
{
	TconstSharedLogEntry entry = m_model->getEntryByIndex( m_table->mapToSource( selected.front().topLeft() ) );
	m_text->setHtml( m_model->getParserModelConfiguration()->getEntryToTextFormater()->formatEntry( entry ) );
}

LogEntryCombinedWidget::~LogEntryCombinedWidget()
{
	qDebug() << " Window deleted.";
}
