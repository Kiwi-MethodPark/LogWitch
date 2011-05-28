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
#include "LogEntryParser_log4cplusSocket.h"

LogfileAnalyser::LogfileAnalyser(QWidget *parent)
    : QMainWindow(parent)
	, m_myFilterDock( NULL )
{
	ui.setupUi(this);

	QString tooltipLog4CPlusPort("Port to listen for log4cplus socket appender");

	QLabel *portLabel = new QLabel("Port: ");
	m_uiLog4cplusPort = new QSpinBox( this );
	m_uiLog4cplusPort->setToolTip(tooltipLog4CPlusPort);
	m_uiLog4cplusPort->setMinimum( 1 );
	m_uiLog4cplusPort->setMaximum(65535);
	m_uiLog4cplusPort->setValue( 9998 );
	ui.ToolbarLog4cplus->addWidget( portLabel);
	m_uiLog4cplusPort_Action = ui.ToolbarLog4cplus->addWidget( m_uiLog4cplusPort );

    QObject::connect(ui.actionOpenDummyLogger, SIGNAL(triggered()),
                     this, SLOT(openDummyLogfile()));
    QObject::connect(ui.actionAddEntries, SIGNAL(triggered()),
                     this, SLOT(moreDummyLogfile()));
    QObject::connect(ui.actionOpen, SIGNAL(triggered()),
                     this, SLOT(openLogfile()));
    QObject::connect(ui.actionOpenLog4cplusServer, SIGNAL(triggered()),
                     this, SLOT(openPort()));
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

void LogfileAnalyser::openPort()
{
	int port = m_uiLog4cplusPort->value();
	boost::shared_ptr<LogEntryParser_log4cplusSocket> socketParser( new LogEntryParser_log4cplusSocket( port ) );

	createWindowsFromParser( socketParser );
}


void LogfileAnalyser::createWindowsFromParser(boost::shared_ptr<LogEntryParser> parser)
{
	boost::shared_ptr<LogEntryTableModel> model( new LogEntryTableModel( parser ) );

	LogEntryCombinedWidget *wnd = new LogEntryCombinedWidget( model, ui.mdiArea );

    wnd->setWindowState(Qt::WindowMaximized );
    wnd->setAttribute(Qt::WA_DeleteOnClose);
	wnd->show();


	/*
	 * We want to open the Dock the first time we ceate a window.
	 * The advantage of doing so is the correct size for the inner
	 * filter tab widget which is set to the dock widget.
	 */
	if( !m_myFilterDock )
	{
		m_myFilterDock = new QDockWidget(tr("FilterSettings"), this);
		m_myFilterDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		wnd->setDockForFilter(m_myFilterDock);
		wnd->getFocused();
		addDockWidget(Qt::RightDockWidgetArea, m_myFilterDock);
	}
	else
	{
		wnd->setDockForFilter(m_myFilterDock);
		wnd->getFocused();
	}


	model->startModel();
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
