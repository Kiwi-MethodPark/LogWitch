#include "logfileanalyser.h"

#include <QtGui>
#include <QtCore/QtCore>

#include <boost/shared_ptr.hpp>

#include "LogData/LogEntryParser_dummy.h"
#include "LogData/LogEntryAttributeFactory.h"

#include "Models/LogEntryTableModel.h"
#include "LogEntryTableWindow.h"

#include "Models/StringCacheTreeModel.h"
#include "LogData/LogEntryParser_Logfile.h"
#include "LogEntryCombinedWidget.h"
#include "LogEntryParser_log4cplusSocket.h"
#include "GUITools/WidgetStateSaver.h"
#include "GUITools/DockWidgetStateSaver.h"
#include "GUITools/SignalMultiplexerStateApplier.h"
#include "ActionRules/FilterRuleSelectionWindow.h"

LogfileAnalyser::LogfileAnalyser(QWidget *parent)
    : QMainWindow(parent)
	, m_myFilterDock( NULL )
    , m_stateSaver( NULL )
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

	m_stateSaver = new WidgetStateSaver(this);
    m_stateSaver->addElementToWatch( &m_signalMultiplexer
            , SignalMultiplexerStateApplier::generate(&m_signalMultiplexer) );

    QObject::connect(ui.actionOpenDummyLogger, SIGNAL(triggered()),
                     this, SLOT(openDummyLogfile()));
    QObject::connect(ui.actionAddEntries, SIGNAL(triggered()),
                     this, SLOT(moreDummyLogfile()));
    QObject::connect(ui.actionOpen, SIGNAL(triggered()),
                     this, SLOT(openLogfile()));
    QObject::connect(ui.actionOpenLog4cplusServer, SIGNAL(triggered()),
                     this, SLOT(openPort()));
    QObject::connect( ui.mdiArea, SIGNAL( subWindowActivated ( QMdiSubWindow *) )
                    , this, SLOT( subWindowActivated( QMdiSubWindow * ) ) );

    m_myFilterRulesDock = new QDockWidget(tr("Filter Rules"), this);
    m_myFilterRulesDock->setAllowedAreas(Qt::AllDockWidgetAreas);
    m_myfilterRuleSelectionWidget = new FilterRuleSelectionWindow( m_myFilterRulesDock );
    m_myFilterRulesDock->setWidget( m_myfilterRuleSelectionWidget );
    addDockWidget(Qt::RightDockWidgetArea, m_myFilterRulesDock);
}

void LogfileAnalyser::subWindowDestroyed( QObject *obj )
{
    qDebug() << "subWindowDestroyed";
    // Window closed, remove docks
    if( ui.mdiArea->subWindowList().size() == 0 )
        m_stateSaver->switchState( NULL );
    else
    {
        QMdiSubWindow *wnd = ui.mdiArea->activeSubWindow();
        if( wnd )
            m_stateSaver->switchState( wnd );
        else
            m_stateSaver->switchState( ui.mdiArea->subWindowList().front() );
    }

    m_stateSaver->deregisterFocusObject( obj, false );
}

void LogfileAnalyser::subWindowActivated( QMdiSubWindow *obj )
{
    qDebug() << "subWindowActivated";
    if( obj != NULL )
        m_stateSaver->switchState( obj );
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
    m_signalMultiplexer.setObject( wnd );
    m_signalMultiplexer.connect( ui.actionClearLogTable, SIGNAL(triggered())
            , wnd, SLOT(clearTable() ) );

    wnd->setWindowState(Qt::WindowMaximized );
    wnd->setAttribute(Qt::WA_DeleteOnClose);
    wnd->setWindowTitle( parser->getName() );
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
        m_stateSaver->addElementToWatch( m_myFilterDock
                , DockWidgetStateSaver::generate() );

		wnd->setDockForFilter(m_myFilterDock);
		addDockWidget(Qt::RightDockWidgetArea, m_myFilterDock);
	}
	else
	{
		wnd->setDockForFilter(m_myFilterDock);
	}

    QObject::connect( wnd, SIGNAL( destroyed ( QObject *) )
                    , this, SLOT( subWindowDestroyed( QObject * ) ) );

	model->startModel();

	TSharedCompiledRulesStateSaver state = wnd->getCompiledRules();
	m_myfilterRuleSelectionWidget->setWindow( state );

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
