#include "logfileanalyser.h"
#include <QMessageBox>
#include <boost/shared_ptr.hpp>

#include "LogData/LogEntryParser_dummy.h"
#include "LogData/LogEntryTable.h"
#include "LogData/LogEntryAttributeFactory.h"

#include "Models/LogEntryTableModel.h"
#include "LogEntryTableWindow.h"

#include <QtGui>
#include <QtCore/QtCore>
#include "Models/StringCacheTreeModel.h"
#include "LogData/LogEntryParser_Logfile.h"

LogfileAnalyser::LogfileAnalyser(QWidget *parent)
    : QMainWindow(parent)
{
	ui.setupUi(this);

    QObject::connect(ui.actionOpenDummyLogger, SIGNAL(triggered()),
                     this, SLOT(openDummyLogfile()));
    QObject::connect(ui.actionAddEntries, SIGNAL(triggered()),
                     this, SLOT(moreDummyLogfile()));
    QObject::connect(ui.actionOpen, SIGNAL(triggered()),
                     this, SLOT(openLogfile()));
}

LogfileAnalyser::~LogfileAnalyser()
{

}

void LogfileAnalyser::openLogfile()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);
    if(dialog.exec())
    {
    	QStringList fileNames = dialog.selectedFiles();
    	createWindowsFromParser( boost::shared_ptr<LogEntryParser>(new LogEntryParser_Logfile( fileNames.first() ) ) );
    }
}


void LogfileAnalyser::createWindowsFromParser(boost::shared_ptr<LogEntryParser> parser)
{
	// This odel must connected to the parser prior to the entryTabModel to get all strings.
	StringCacheTreeModel *strModel = new StringCacheTreeModel( &parser->getLogEntryAttributeFactory()->getCache(0) );
	StringCacheTreeModel *str2Model = new StringCacheTreeModel( &parser->getLogEntryAttributeFactory()->getCache(1), "\\." );

	boost::shared_ptr<LogEntryTableModel> model( new LogEntryTableModel( parser ) );
	m_model = model;

	LogEntryTableWindow *wnd = new LogEntryTableWindow( model );
	ui.mdiArea->addSubWindow( wnd );
	wnd->show();

    QTreeView *view = new QTreeView;
	view->setModel(strModel);
	view->setWindowTitle(QObject::tr("Simple Tree Model"));
	ui.mdiArea->addSubWindow( view );
	view->show();

    view = new QTreeView;
	view->setModel(str2Model);
	view->setWindowTitle(QObject::tr("Source Model"));
	ui.mdiArea->addSubWindow( view );
	view->show();
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

	// This odel must connected to the parser prior to the entryTabModel to get all strings.
	StringCacheTreeModel *strModel = new StringCacheTreeModel( &parser->getLogEntryAttributeFactory()->getCache(0) );
	StringCacheTreeModel *str2Model = new StringCacheTreeModel( &parser->getLogEntryAttributeFactory()->getCache(1), "\\." );

	boost::shared_ptr<LogEntryTableModel> model( new LogEntryTableModel( parser ) );
	m_model = model;

	LogEntryTableWindow *wnd = new LogEntryTableWindow( model );
	ui.mdiArea->addSubWindow( wnd );
	wnd->show();


    QTreeView *view = new QTreeView;
	view->setModel(strModel);
	view->setWindowTitle(QObject::tr("Simple Tree Model"));
	ui.mdiArea->addSubWindow( view );
	view->show();

    view = new QTreeView;
	view->setModel(str2Model);
	view->setWindowTitle(QObject::tr("Source Model"));
	ui.mdiArea->addSubWindow( view );
	view->show();

}

void LogfileAnalyser::moreDummyLogfile()
{
	if( !m_parser )
		return;

	m_parser->addEntries( 100 );
}
