#include "logfileanalyser.h"
#include <QMessageBox>
#include <boost/shared_ptr.hpp>

#include "LogData/LogEntryParser_dummy.h"
#include "LogData/LogEntryTable.h"

#include "Models/LogEntryTableModel.h"
#include "LogEntryTableWindow.h"

#include <QtGui>
#include <QtCore/QtCore>

LogfileAnalyser::LogfileAnalyser(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);

    QObject::connect(ui.actionOpenDummyLogger, SIGNAL(triggered()),
                     this, SLOT(openDummyLogfile()));
    QObject::connect(ui.actionAddEntries, SIGNAL(triggered()),
                     this, SLOT(moreDummyLogfile()));
}

LogfileAnalyser::~LogfileAnalyser()
{

}

void LogfileAnalyser::openDummyLogfile()
{
//	QMessageBox msgBox;
//	msgBox.setText("Dummy log entries will be loaded ...");
//	msgBox.setStandardButtons(QMessageBox::Ok );
//	msgBox.setDefaultButton(QMessageBox::Ok);
//	msgBox.exec();

	// Create table with log entries and a new model for this
	boost::shared_ptr<LogEntryParser_dummy> parser( new LogEntryParser_dummy );
	m_parser = parser;

	boost::shared_ptr<LogEntryTableModel> model( new LogEntryTableModel( parser ) );
	m_model = model;

	LogEntryTableWindow *wnd = new LogEntryTableWindow( model );
	ui.mdiArea->addSubWindow( wnd );
	wnd->show();
}

void LogfileAnalyser::moreDummyLogfile()
{
	if( !m_parser )
		return;

	m_parser->addEntries( 100 );
}
