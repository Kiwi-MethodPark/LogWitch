#include "logfileanalyser.h"

#include <iostream>

#include <QtGui>
#include <QApplication>

#include "LogData/LogEntry.h"
#include "Types.h"


int main(int argc, char *argv[])
{
	 qRegisterMetaType<TSharedLogEntry>("TSharedLogEntry");
	 qRegisterMetaType<TSharedConstQString>("TSharedConstQString");


    QApplication a(argc, argv);
    LogfileAnalyser w;
    w.show();
    return a.exec();
}
