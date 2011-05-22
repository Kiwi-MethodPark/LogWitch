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

LogEntryCombinedWidget::LogEntryCombinedWidget( boost::shared_ptr<LogEntryTableModel> model )
	:QSplitter( Qt::Vertical )
	 , m_model( model )
{
	m_table = new LogEntryTableWindow( model, this );
	m_text = new QTextEdit("<b>Log Message viewer</b>",this);

    QObject::connect(m_table->selectionModel(), SIGNAL(selectionChanged ( const QItemSelection & , const QItemSelection & )),
                     SLOT(newSelection ( const QItemSelection &, const QItemSelection & )));

    addWidget( m_table );
    addWidget( m_text );

    this->setStretchFactor( 0, 20 );
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
}
