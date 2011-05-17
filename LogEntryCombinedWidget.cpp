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

LogEntryCombinedWidget::LogEntryCombinedWidget( boost::shared_ptr<LogEntryTableModel> model )
	:QSplitter( Qt::Vertical )
	 , m_model( model )
{
	LogEntryTableWindow *table = new LogEntryTableWindow( model, this );
	m_text = new QTextEdit("<b>Log Message viewer</b>",this);

    QObject::connect(table->selectionModel(), SIGNAL(selectionChanged ( const QItemSelection & , const QItemSelection & )),
                     SLOT(newSelection ( const QItemSelection &, const QItemSelection & )));

    addWidget( table );
    addWidget( m_text );
}

void LogEntryCombinedWidget::newSelection ( const QItemSelection & selected, const QItemSelection & )
{
	qDebug() << "New selection of size: " << selected.size();

	TconstSharedLogEntry entry = m_model->getEntryByIndex( selected.front().topLeft() );

	EntryToTextFormater fmt;

	m_text->setHtml( fmt.formatEntry( entry ) );
}

LogEntryCombinedWidget::~LogEntryCombinedWidget()
{
}
