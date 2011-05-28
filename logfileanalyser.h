#ifndef LOGFILEANALYSER_H
#define LOGFILEANALYSER_H

#include <boost/shared_ptr.hpp>

#include <QtGui/QMainWindow>

#include "ui_logfileanalyser.h"

class LogEntryParser_dummy;
class LogEntryTableModel;
class LogEntryTable;
class LogEntryParser;
class QDockWidget;
class QSpinBox;

class LogfileAnalyser : public QMainWindow
{
    Q_OBJECT

public:
    LogfileAnalyser(QWidget *parent = 0);
    ~LogfileAnalyser();

    void createWindowsFromParser(boost::shared_ptr<LogEntryParser> parser);

private:
    Ui::LogfileAnalyserClass ui;

    boost::shared_ptr<LogEntryParser_dummy> m_parser;

    QDockWidget *m_myFilterDock;

    QSpinBox *m_uiLog4cplusPort;
    QAction * m_uiLog4cplusPort_Action;

private slots:
  	void openDummyLogfile();

  	void moreDummyLogfile();

  	void openLogfile();

  	void openPort();
};

#endif // LOGFILEANALYSER_H
