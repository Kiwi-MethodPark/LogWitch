/*
 * LogDataTypeRegistration.cpp
 *
 *  Created on: 12 Mar 2016
 *      Author: sven
 */

#include <LogData/LogDataTypeRegistration.h>

#include <QtCore/QMetaType>

#include "LogData/LogEntry.h"
#include "LogData/NewLogEntryMessage.h"

void registerLogDataTypes()
{
  qRegisterMetaType<TSharedLogEntry>("TSharedLogEntry");
  qRegisterMetaType<TSharedNewLogEntryMessage>("TSharedNewLogEntryMessage");
  qRegisterMetaType<TSharedNewLogEntryMessage>("TconstSharedNewLogEntryMessage");
}
