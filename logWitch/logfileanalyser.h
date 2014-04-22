#ifndef LOGFILEANALYSER_H
#define LOGFILEANALYSER_H

#include <boost/shared_ptr.hpp>

#include <QtGui/QMainWindow>

#include "ui_logfileanalyser.h"
#include "GUITools/SignalMultiplexer.h"

class HelpAssistant;

class LogEntryParser_dummy;
class LogEntryTableModel;
class LogEntryTable;
class LogEntryParser;
class QDockWidget;
class QSpinBox;
class WidgetStateSaver;
class QMdiSubWindow;
class FilterRuleSelectionWindow;

class LogfileAnalyser : public QMainWindow
{
  Q_OBJECT

public:
  LogfileAnalyser(QWidget *parent = 0);
  ~LogfileAnalyser();

  bool initParser();

  void createWindowsFromParser(boost::shared_ptr<LogEntryParser> parser, bool alreadyInitialized = false);

public slots:
  void subWindowActivated(QMdiSubWindow *);
  void subWindowDestroyed(QObject *obj);
  void showDocumentation();

private:
  Ui::LogfileAnalyserClass ui;

  boost::shared_ptr<LogEntryParser_dummy> m_parser;

  QDockWidget *m_myFilterDock;
  QDockWidget *m_myFilterRulesDock;

  FilterRuleSelectionWindow *m_myfilterRuleSelectionWidget;

  QSpinBox *m_uiLog4cplusPort;
  QAction * m_uiLog4cplusPort_Action;

  WidgetStateSaver *m_stateSaver;
  SignalMultiplexer m_signalMultiplexer;

  boost::shared_ptr<HelpAssistant> m_helpAssistant;

private slots:
  void openDummyLogfile();

  void moreDummyLogfile();

  void openLogfile();

  void openPort();

  void exportLogfile();
};

#endif // LOGFILEANALYSER_H
