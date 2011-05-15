#include "logfileanalyser.h"

#include <iostream>

#include <QtGui>
#include <QApplication>

#include "LogData/LogEntry.h"


int main(int argc, char *argv[])
{
	 int id = qRegisterMetaType<TSharedLogEntry>("TSharedLogEntry");
	 std::cout << " TSharedLogEntry registered to id: " << id << std::endl;

    QApplication a(argc, argv);
    LogfileAnalyser w;
    w.show();
    return a.exec();
}
