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

	QMdiSubWindow *wnd = new  QMdiSubWindow( ui.mdiArea );
	LogEntryCombinedWidget *wid = new LogEntryCombinedWidget( model );

	wnd->setWidget( wid );
	wnd->setWindowState(Qt::WindowMaximized );
    wnd->setAttribute(Qt::WA_DeleteOnClose);
    wnd->resize( 800, 800 );
	wnd->show();

	ui.mdiArea->addSubWindow( wnd );

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
			wid->addFilter( strModel->getFilter() );

		view->setModel(strModel);
		tabs->addTab( view, parser->getParserModelConfiguration()->getLogEntryAttributeFactory()->getDescription(attr));
		view->show();
	}

	addDockWidget(Qt::RightDockWidgetArea, dock);

	m_model->startModel();
}

void LogfileAnalyser::openDummyLogfile()
{
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
