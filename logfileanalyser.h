#ifndef LOGFILEANALYSER_H
#define LOGFILEANALYSER_H

#include <QtGui/QMainWindow>
#include "ui_logfileanalyser.h"
#include <boost/shared_ptr.hpp>

class LogEntryParser_dummy;
class LogEntryTableModel;
class LogEntryTable;
class LogEntryParser;

class LogfileAnalyser : public QMainWindow
{
    Q_OBJECT

public:
    LogfileAnalyser(QWidget *parent = 0);
    ~LogfileAnalyser();

    void createWindowsFromParser(boost::shared_ptr<LogEntryParser> parser);

private:
    Ui::LogfileAnalyserClass ui;

    boost::shared_ptr<LogEntryTableModel> m_model;
    boost::shared_ptr<LogEntryParser_dummy> m_parser;

private slots:
  	void openDummyLogfile();

  	void moreDummyLogfile();

  	void openLogfile();
};

#endif // LOGFILEANALYSER_H
