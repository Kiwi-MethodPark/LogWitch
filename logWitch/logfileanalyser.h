#ifndef LOGFILEANALYSER_H
#define LOGFILEANALYSER_H

#include <boost/shared_ptr.hpp>

#include <QMainWindow>

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

/**
 * Interface to signal a new parser action which requests the main application ot
 * open a new window
 */
class ParserActionInterface
{
public:
	virtual ~ParserActionInterface() = default;

	/**
	 * A new parser has been created an shall be passed to the application
	 */
	virtual void newParser(boost::shared_ptr<LogEntryParser> parser, bool alreadyInitialized = false) = 0;
};

class Log4cplusGUIIntegration: public QObject
{
	Q_OBJECT
public:
	Log4cplusGUIIntegration( ParserActionInterface* parserActionIfc );

	virtual ~Log4cplusGUIIntegration();

	QToolBar* getToolbar() {return m_toolbar; }

private slots:
	void openPort();

private:
	ParserActionInterface* m_parserActionIfc;

	QToolBar* m_toolbar;
	QSpinBox* m_port;
};

class LogfileAnalyser : public QMainWindow, public ParserActionInterface
{
  Q_OBJECT

public:
  LogfileAnalyser(QWidget *parent = 0);
  ~LogfileAnalyser();

  bool initParser();

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

  // TODO: This is a temporary hack for refactoring, later there shall be only abstract types
  Log4cplusGUIIntegration* m_log4cplusIntegration;

private slots:
  void openDummyLogfile();

  void moreDummyLogfile();

  void openLogfile();

  void exportLogfile();

public slots:
  void openLogfile(const QString & fileNames);
};

#endif // LOGFILEANALYSER_H
