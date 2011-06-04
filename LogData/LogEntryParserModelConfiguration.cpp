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

QString LogEntryParserModelConfiguration::getHierarchySplitString( int idx ) const
{
    if( idx >= 0 && idx < m_hierarchySplitstrings.size() )
        return m_hierarchySplitstrings[idx];
    else
        return "";
}

void LogEntryParserModelConfiguration::setHierarchySplitString( int idx, const QString & fmt)
{
    if( idx >= m_hierarchySplitstrings.size() )
        m_hierarchySplitstrings.resize(idx+1);

    m_hierarchySplitstrings[idx] = fmt;
}


int LogEntryParserModelConfiguration::getFieldWidthHint( int idx ) const
{
    if( idx < (int)m_filedWidthHints.size() )
        return m_filedWidthHints[idx] < 0 ? 150 : m_filedWidthHints[idx];
    else
        return 150;
}

void LogEntryParserModelConfiguration::setFieldWidthHint( int idx, int width )
{
    while( idx >= (int)m_filedWidthHints.size() )
        m_filedWidthHints.push_back(-1);

    m_filedWidthHints[idx] = width;
}
