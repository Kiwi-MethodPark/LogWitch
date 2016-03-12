#include "logfileanalyser.h"

#include <QtGui>
#include <QtCore/QtCore>

#include <boost/shared_ptr.hpp>
#include <boost/smart_ptr/make_shared.hpp>

#include "LogData/LogEntryParser_dummy.h"
#include "LogData/LogEntryFactory.h"
#include "LogData/ParserStreamGetterFile.h"

#include "Models/LogEntryTableModel.h"
#include "LogEntryTableWindow.h"

#include "Models/StringCacheTreeModel.h"
#include "LogData/LogEntryParser_Logfile.h"
#include "LogData/LogEntryParser_LogfileLWI.h"
#include "LogData/LogEntryParser_log4cplusSocket.h"
#include "GUITools/WidgetStateSaver.h"
#include "GUITools/GetSetStateSaver.hxx"
#include "GUITools/SignalMultiplexerStateApplier.h"
#include "ActionRules/FilterRuleSelectionWindow.h"
#include "Help/HelpAssistant.h"

LogfileAnalyser::LogfileAnalyser(QWidget *parent)
  : QMainWindow(parent),
    m_myFilterDock( NULL),
    m_stateSaver( NULL),
    m_helpAssistant(new HelpAssistant)
{
  ui.setupUi(this);

  QString tooltipLog4CPlusPort("Port to listen for log4cplus socket appender");

  QLabel *portLabel = new QLabel("Port: ");
  m_uiLog4cplusPort = new QSpinBox(this);
  m_uiLog4cplusPort->setToolTip(tooltipLog4CPlusPort);
  m_uiLog4cplusPort->setMinimum(1);
  m_uiLog4cplusPort->setMaximum(65535);
  m_uiLog4cplusPort->setValue(9998);
  ui.ToolbarLog4cplus->addWidget(portLabel);
  m_uiLog4cplusPort_Action = ui.ToolbarLog4cplus->addWidget(m_uiLog4cplusPort);

  m_stateSaver = new WidgetStateSaver(this);
  m_stateSaver->addElementToWatch(&m_signalMultiplexer,
                                  SignalMultiplexerStateApplier::generate(&m_signalMultiplexer));

  QObject::connect(ui.actionHelp, SIGNAL(triggered()), this, SLOT(showDocumentation()));
  QObject::connect(ui.actionOpenDummyLogger, SIGNAL(triggered()), this, SLOT(openDummyLogfile()));
  QObject::connect(ui.actionAddEntries, SIGNAL(triggered()), this, SLOT(moreDummyLogfile()));
  QObject::connect(ui.actionOpen, SIGNAL(triggered()), this, SLOT(openLogfile()));
  QObject::connect(ui.actionExportLogfile, SIGNAL(triggered()), this, SLOT(exportLogfile()));
  QObject::connect(ui.actionOpenLog4cplusServer, SIGNAL(triggered()), this, SLOT(openPort()));
  QObject::connect(ui.mdiArea, SIGNAL(subWindowActivated ( QMdiSubWindow *)), this,
                   SLOT(subWindowActivated( QMdiSubWindow * )));

  m_myFilterRulesDock = new QDockWidget(tr("Filter Rules"), this);
  m_myFilterRulesDock->setAllowedAreas(Qt::AllDockWidgetAreas);
  m_myfilterRuleSelectionWidget = new FilterRuleSelectionWindow(m_myFilterRulesDock);
  m_myFilterRulesDock->setWidget(m_myfilterRuleSelectionWidget);
  addDockWidget(Qt::RightDockWidgetArea, m_myFilterRulesDock);

  m_stateSaver->addElementToWatch(m_myfilterRuleSelectionWidget,
                                  GetSetStateSaver<FilterRuleSelWndStateSaverTypes>::generate());
  m_stateSaver->addElementToWatch(ui.actionCapture,
                                  GetSetStateSaver<QQctionCheckedSaverTypes>::generate());

}

void LogfileAnalyser::subWindowDestroyed(QObject *obj)
{
  qDebug() << "subWindowDestroyed";
  // Window closed, remove docks
  if (ui.mdiArea->subWindowList().size() == 0)
    m_stateSaver->switchState( NULL);
  else
  {
    QMdiSubWindow *wnd = ui.mdiArea->activeSubWindow();
    if (wnd)
      m_stateSaver->switchState(wnd);
    else
      m_stateSaver->switchState(ui.mdiArea->subWindowList().front());
  }

  m_stateSaver->deregisterFocusObject(obj, false);
}

void LogfileAnalyser::subWindowActivated(QMdiSubWindow *obj)
{
  qDebug() << "subWindowActivated";
  if (obj != NULL)
    m_stateSaver->switchState(obj);
}

void LogfileAnalyser::showDocumentation()
{
  qDebug() << "ShuwDocumentation";

  m_helpAssistant->showDocumentation("index.html");
}

LogfileAnalyser::~LogfileAnalyser()
{

}

void LogfileAnalyser::openLogfile()
{
  QFileDialog dialog;
  dialog.setFileMode(QFileDialog::ExistingFile);
  if (dialog.exec())
  {
    QStringList fileNames = dialog.selectedFiles();

    boost::shared_ptr<LogEntryParser> parser( new LogEntryParser_LogfileLWI(fileNames.first()) );
    if (!parser->initParser())
    {
      qDebug() << " LWI-Parser failed: " << parser->getInitError();
      parser.reset( new LogEntryParser_Logfile( boost::make_shared<ParserStreamGetterFile>( fileNames.first() ) ) );
      if (parser->initParser())
        createWindowsFromParser( parser, true);
      else
      {
        QMessageBox msgBox;
        QString errorText;
        errorText += tr("Error while initializing parser: \n") + parser->getInitError();
        msgBox.setText(errorText);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);
        msgBox.exec();
      }
    }
    else
    {
      createWindowsFromParser( parser, true);
    }
  }
}

void LogfileAnalyser::openPort()
{
  int port = m_uiLog4cplusPort->value();
  boost::shared_ptr<LogEntryParser_log4cplusSocket> socketParser(
    new LogEntryParser_log4cplusSocket(port));

  createWindowsFromParser(socketParser);
}

void LogfileAnalyser::createWindowsFromParser(boost::shared_ptr<LogEntryParser> parser, bool alreadyInitialized)
{
  if (!alreadyInitialized && !parser->initParser())
  {
    // Parser has an error while init, so view message box to inform user and do
    // not create a new window.
    QMessageBox msgBox;
    QString errorText;
    errorText += tr("Error while initializing parser: \n") + parser->getInitError();
    msgBox.setText(errorText);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Ok);
    msgBox.exec();
    return;
  }

  boost::shared_ptr<LogEntryTableModel> model(new LogEntryTableModel(parser));

  LogEntryTableWindow *wnd = new LogEntryTableWindow(model, ui.mdiArea);
  m_signalMultiplexer.setObject(wnd);
  m_signalMultiplexer.connect(ui.actionClearLogTable, SIGNAL(triggered()), wnd, SLOT(clearTable()));
  m_signalMultiplexer.connect(ui.actionCapture, SIGNAL(toggled(bool)), wnd, SLOT(capture(bool)));

  wnd->setWindowState(Qt::WindowMaximized);
  wnd->setAttribute(Qt::WA_DeleteOnClose);
  wnd->setWindowTitle(parser->getName());
  wnd->show();

  ui.actionCapture->setChecked(true);

  /*
   * We want to open the Dock the first time we create a window.
   * The advantage of doing so is the correct size for the inner
   * filter tab widget which is set to the dock widget.
   */
  if (!m_myFilterDock)
  {
    m_myFilterDock = new QDockWidget(tr("FilterSettings"), this);
    m_myFilterDock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_stateSaver->addElementToWatch(m_myFilterDock, DockWidgetStateSaver::generate());

    wnd->setDockForFilter(m_myFilterDock);
    addDockWidget(Qt::RightDockWidgetArea, m_myFilterDock);
  }
  else
  {
    wnd->setDockForFilter(m_myFilterDock);
  }

  QObject::connect(wnd, SIGNAL(destroyed ( QObject *)), this,
                   SLOT(subWindowDestroyed( QObject * )));

  model->startModel();

  TSharedCompiledRulesStateSaver state = wnd->getCompiledRules();
  m_myfilterRuleSelectionWidget->setWindow(state);

}

void LogfileAnalyser::openDummyLogfile()
{
  // Create table with log entries and a new model for this
  boost::shared_ptr<LogEntryParser_dummy> parser(new LogEntryParser_dummy);
  m_parser = parser;

  createWindowsFromParser(parser);
}

void LogfileAnalyser::moreDummyLogfile()
{
  if (!m_parser)
    return;

  m_parser->addEntries(100);
}

void LogfileAnalyser::exportLogfile()
{
  LogEntryTableWindow *wnd = NULL;
  wnd = dynamic_cast<LogEntryTableWindow *>(ui.mdiArea->activeSubWindow());

  if (wnd)
  {
    QString fname;
    fname += "exportedLogfile " + QDateTime::currentDateTime().toString("yyyyMMdd hhmm") + ".log";

    QString fileName = QFileDialog::getSaveFileName(this, tr("Export Logfile"), fname,
                                                    tr("Logfile (*.log *.txt)"));

    if (fileName.length())
      wnd->exportLogfile(fileName);
  }
}
