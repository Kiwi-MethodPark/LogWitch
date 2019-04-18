#ifndef LOGFILEANALYSER_H
#define LOGFILEANALYSER_H

#include <boost/shared_ptr.hpp>

#include <QtCore>
#include <QMainWindow>

#include "ui_logfileanalyser.h"
#include "GUITools/SignalMultiplexer.h"
#include "ParserActionInterface.h"

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

namespace logwitch { namespace plugins {
	class LogSourcePlugin;
}}

class LogfileAnalyser : public QMainWindow, public logwitch::ParserActionInterface
{
  Q_OBJECT

public:
  LogfileAnalyser(QWidget *parent = 0);
  ~LogfileAnalyser();

  void newParser(boost::shared_ptr<LogEntryParser> parser, bool alreadyInitialized) override;

public slots:
  void subWindowActivated(QMdiSubWindow *);
  void subWindowDestroyed(QObject *obj);
  void showDocumentation();

private:
  void createWindowsFromParser(boost::shared_ptr<LogEntryParser> parser, bool alreadyInitialized = false);

  Ui::LogfileAnalyserClass ui;

  boost::shared_ptr<LogEntryParser_dummy> m_parser;

  QDockWidget *m_myFilterDock;
  QDockWidget *m_myFilterRulesDock;

  FilterRuleSelectionWindow *m_myfilterRuleSelectionWidget;

  WidgetStateSaver *m_stateSaver;
  SignalMultiplexer m_signalMultiplexer;

  boost::shared_ptr<HelpAssistant> m_helpAssistant;

  void loadPlugins();
  void loadPlugins(QDir basePath);

  std::list< logwitch::plugins::LogSourcePlugin* > m_logSourcePlugins;

private slots:
  void openDummyLogfile();

  void moreDummyLogfile();

  void openLogfile();

  void exportLogfile();

public slots:
  void openLogfile(const QString & fileNames);
};

#endif // LOGFILEANALYSER_H
