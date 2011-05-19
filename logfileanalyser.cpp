#include "logfileanalyser.h"
#include <QMessageBox>
#include <boost/shared_ptr.hpp>

#include "LogData/LogEntryParser_dummy.h"
#include "LogData/LogEntryAttributeFactory.h"

#include "Models/LogEntryTableModel.h"
#include "LogEntryTableWindow.h"

#include <QtGui>
#include <QtCore/QtCore>
#include "Models/StringCacheTreeModel.h"
#include "LogData/LogEntryParser_Logfile.h"
#include "LogEntryCombinedWidget.h"

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
	boost::shared_ptr<LogEntryTableModel> model( new LogEntryTableModel( parser ) );
	m_model = model;

	LogEntryCombinedWidget *wnd = new LogEntryCombinedWidget( model );
	ui.mdiArea->addSubWindow( wnd );
	wnd->show();

	QDockWidget *dock = new QDockWidget(tr("FilterSettings"), this);
	dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

	QTabWidget *tabs = new QTabWidget(dock);
	dock->setWidget( tabs );

	int attributes = parser->getParserModelConfiguration()->getLogEntryAttributeFactory()->getNumberOfFields();
	for(int attr = 0; attr < attributes; attr++ )
	{
		QTreeView *view = new QTreeView;
		StringCacheTreeModel *strModel = new StringCacheTreeModel(view
				, &parser->getParserModelConfiguration()->getLogEntryAttributeFactory()->getCache(attr)
				, attr
				, parser->getParserModelConfiguration()->getHierarchySplitString(attr) );

		if( strModel->getFilter() )
			wnd->addFilter( strModel->getFilter() );

		view->setModel(strModel);
		view->show();
		tabs->addTab( view, parser->getParserModelConfiguration()->getLogEntryAttributeFactory()->getDescription(attr));
	}

	addDockWidget(Qt::RightDockWidgetArea, dock);

	m_model->startModel();


/*
    view = new QTreeView;
	view->setModel(str2Model);
	view->setWindowTitle(QObject::tr("Source Model"));
	ui.mdiArea->addSubWindow( view );
	view->show();
	*/
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

	createWindowsFromParser( parser );
}

void LogfileAnalyser::moreDummyLogfile()
{
	if( !m_parser )
		return;

	m_parser->addEntries( 100 );
}
