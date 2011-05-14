#ifndef LOGFILEANALYSER_H
#define LOGFILEANALYSER_H

#include <QtGui/QMainWindow>
#include "ui_logfileanalyser.h"

class LogfileAnalyser : public QMainWindow
{
    Q_OBJECT

public:
    LogfileAnalyser(QWidget *parent = 0);
    ~LogfileAnalyser();

private:
    Ui::LogfileAnalyserClass ui;
};

#endif // LOGFILEANALYSER_H
