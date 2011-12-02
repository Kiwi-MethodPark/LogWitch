#include "logfileanalyser.h"

#include <iostream>

#include <QtGui>
#include <QApplication>

#include "LogData/LogEntry.h"
#include "LogData/NewLogEntryMessage.h"
#include "Types.h"


int main(int argc, char *argv[])
{
    qRegisterMetaType<TSharedLogEntry>("TSharedLogEntry");
    qRegisterMetaType<TSharedConstQString>("TSharedConstQString");
    qRegisterMetaType<TSharedNewLogEntryMessage>("TSharedNewLogEntryMessage");
    qRegisterMetaType<TSharedNewLogEntryMessage>("TconstSharedNewLogEntryMessage");

    QCoreApplication::setOrganizationName("Steckmann");
    QCoreApplication::setOrganizationDomain("steckmann.de");
    QCoreApplication::setApplicationName("LogfileAnalyser");

    QApplication a(argc, argv);
    LogfileAnalyser w;
    w.show();
    return a.exec();
}
