/*
 * LogEntryParserModelConfiguration.cpp
 *
 *  Created on: May 18, 2011
 *      Author: sven
 */

#include "LogEntryParserModelConfiguration.h"
#include "EntryToTextFormaterDefault.h"

LogEntryParserModelConfiguration::LogEntryParserModelConfiguration()
    : m_formater( new EntryToTextFormaterDefault )
{

}

LogEntryParserModelConfiguration::~LogEntryParserModelConfiguration()
{

}
