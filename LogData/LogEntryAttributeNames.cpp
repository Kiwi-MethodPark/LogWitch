/*
 * LogEntryAttributeNames.cpp
 *
 *  Created on: Jun 23, 2011
 *      Author: sven
 */

#include "LogEntryAttributeNames.h"

LogEntryAttributeNames::LogEntryAttributeNames()
:attDescNumber("number",tr("Number"))
,attDescTimestamp("timestamp",tr("Timestamp"))
,attDescMessage("message",tr("Log Message"))
,attDescLoglevel("loglevel",tr("Loglevel"))
,attDescNDC("ndc",tr("Nested Diagnostic content"))
,attDescThread("thread",tr("Thread"))
,attDescLogger("logger",tr("Logger"))
,attDescFileSource("fsource",tr("File Source"))
{
}

